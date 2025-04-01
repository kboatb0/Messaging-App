#include "server.h"

Server::Server()
{
    startServer();
}


Server::~Server()
{
    closesocket(serverSocket);
    closesocket(clientSocket);
}


void Server::startServer()
{
    // Create the server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Error! Socket creation failed: " << WSAGetLastError() << '\n';
        WSACleanup();
        return;
    }

    // Configure the server address structure
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080); // Use port 8080
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Listen on all available interfaces

    // Bind the socket to the server address
    if (bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }
    else {
        std::cout << "Connecting to server...\n";
    }

    // Start listening for incoming connections
    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        std::cerr << "Failed to listen: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }
    else {
        std::cout << "Server is listening...\n";
    }

    acceptClients();
}


void Server::handleClients(SOCKET sockClient)
{
    char buffer[4096];

    while (true) {
        memset(buffer, 0, sizeof(buffer));

        // Receive message from client
        int bytesReceived = recv(sockClient, buffer, sizeof(buffer) - 1, 0);

        if (bytesReceived <= 0) {
            std::cerr << "Disconnected\n";
            break;
        }
        else {
            std::cout << "Received\n";
            std::cout << buffer << std::endl;
        }

        // Broadcast the received message to all connected clients
        std::lock_guard<std::mutex> lock(clientMutex);
        for (SOCKET sock : clientsContainer) {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            send(sock, buffer, strlen(buffer), 0);
        }
    }

    // Remove the client socket from the list of active clients
    {
        std::lock_guard<std::mutex> lock(clientMutex);
        clientsContainer.erase(std::remove(clientsContainer.begin(), clientsContainer.end(), sockClient), clientsContainer.end());
    }

    // Close the client's socket after they disconnect
    closesocket(clientSocket);
}


void Server::acceptClients()
{
    while (true) {
        // Accept an incoming client connection
        SOCKET newClientSocket = accept(serverSocket, nullptr, nullptr);
        if (newClientSocket == INVALID_SOCKET) {
            std::cerr << "Failed to accept: " << WSAGetLastError() << std::endl;
            continue;
        }

        // Add the new client socket to the container
        {
            std::lock_guard<std::mutex> lock(clientMutex);
            clientsContainer.push_back(newClientSocket);
        }

        // Create a new thread to handle this client
        std::thread handleThread(&Server::handleClients, this, newClientSocket);
        handleThread.detach();
    }
}