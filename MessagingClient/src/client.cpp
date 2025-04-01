#include "client.h"

Client::Client()
{
    connectToServer();
}


Client::~Client()
{
    closesocket(clientSocket);
}


void Client::connectToServer()
{
    // Configure server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080); // Use port 8080
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Connect to localhost

    while (true) {
        // Create a new client socket
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Socket creation failed: " << WSAGetLastError() << '\n';
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }

        // Attempt to connect to the server
        if (connect(clientSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
            std::cerr << "Connection failed: " << WSAGetLastError() << '\n';
            closesocket(clientSocket);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        else {
            std::cout << "Connected to server!\n";
            break;
        }
    }

    std::string username;
    std::cout << "Enter username: ";
    std::cin >> username;

    while (send(clientSocket, username.c_str(), username.length(), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send username: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Reconnecting...\n";
        connectToServer();
    }
}



void Client::sendMessage()
{
    std::string message;

    while (true) {
        std::cout << "> ";
        std::cout.flush();
        std::getline(std::cin, message);

        // Send message to server
        if (send(clientSocket, message.c_str(), message.size(), 0) == SOCKET_ERROR) {
            std::cerr << "Failed to send message: " << WSAGetLastError() << std::endl;
            closesocket(clientSocket);
            std::cout << "Reconnecting...\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
            connectToServer();
        }
    }
}


std::mutex consoleMutex;

void Client::receiveMessage()
{
    char buffer[4096];

    while (true) {
        memset(buffer, 0, sizeof(buffer));

        // Receive message from server
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

        if (bytesReceived <= 0) {
            std::cerr << "\nLost connection to server: " << WSAGetLastError() << std::endl;
            closesocket(clientSocket);
            std::cout << "Reconnecting...\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
            connectToServer(); // Attempt to reconnect
        }
        else {
            buffer[bytesReceived] = '\0';

            // Prevent overlapping console output
            {
                consoleMutex.lock();
                std::cout << "\r" << std::string(150, ' ') << "\r"; // Clear current input line
                std::cout << buffer << std::endl;  // Display received message
                std::cout << "> " << std::flush;   // Restore input prompt
                consoleMutex.unlock();
            }
        }
    }
}
