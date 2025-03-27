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
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET) {
		std::cerr << "Error! Socket creation failed: " << WSAGetLastError() << '\n';
		WSACleanup();
		return;
	}

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	if (bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return;
	}
	else {
		std::cout << "Connecting to server...\n";
	}

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
