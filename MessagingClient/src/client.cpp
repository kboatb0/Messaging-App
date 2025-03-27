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
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    while (true) {
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Socket creation failed: " << WSAGetLastError() << '\n';
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }

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
    std::getline(std::cin, username);

    bool sent = false;
    while (!sent) {
        if (send(clientSocket, username.c_str(), username.size(), 0) == SOCKET_ERROR) {
            std::cerr << "Failed to send USERNAME: " << WSAGetLastError() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            closesocket(clientSocket);
            connectToServer();
            return;
        }
        else {
            sent = true;
        }
    }
}


void Client::sendMessage()
{
    std::string message;

    while (true) {
        std::cout << "Type..: ";
        std::getline(std::cin, message);

        if (send(clientSocket, message.c_str(), message.size(), 0) == SOCKET_ERROR) {
            std::cerr << "Failed to send message: " << WSAGetLastError() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            closesocket(clientSocket);
        }
    }
}


void Client::receiveMessage()
{
    char buffer[4096];

    while (true) {
        memset(buffer, 0, sizeof(buffer));

        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesReceived <= 0) {
            std::cerr << "Client disconnected from server: " << WSAGetLastError() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            closesocket(clientSocket);
        }
        else {
            std::string message(buffer, bytesReceived);
            std::cout << message << std::endl;
        }
    }
}