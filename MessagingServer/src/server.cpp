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


void Server::handleClients(SOCKET sockclient)
{
	char buffer[4096];

	while (true) {
		memset(buffer, 0, sizeof(buffer));

		int bytesReceived = recv(sockclient, buffer, strlen(buffer), 0);

		if (bytesReceived <= 0) {
			std::cerr << "Disconnected\n";
			break;
		}
		else {
			std::cout << "Received\n";
			std::cout << buffer << std::endl;
		}

		std::lock_guard<std::mutex> lock(clientMutex);
		for (SOCKET sock : clientsContainer) {
			//if (sock != clientSocket) {
			send(sockclient, buffer, strlen(buffer), 0);
			//}
		}
	}

	{
		std::lock_guard<std::mutex> lock(clientMutex);
		clientsContainer.erase(std::remove(clientsContainer.begin(), clientsContainer.end(), clientSocket), clientsContainer.end());
	}

	closesocket(clientSocket);
}


void Server::acceptClients()
{
	while (true) {
		SOCKET newClientSocket = accept(serverSocket, nullptr, nullptr);
		if (newClientSocket == INVALID_SOCKET) {
			std::cerr << "Failed to accept: " << WSAGetLastError() << std::endl;
		}

		{
			std::lock_guard<std::mutex> lock(clientMutex);
			clientsContainer.push_back(newClientSocket);
		}

		std::thread handleThread(&Server::handleClients, this, newClientSocket);
		handleThread.detach();
	}
}