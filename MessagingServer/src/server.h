#pragma once

#include <winsock2.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <algorithm>

class Server {
public:
	Server();
	~Server();

	void startServer();
	void handleClients(SOCKET sockclient);
	void acceptClients();

private:
	SOCKET serverSocket;
	SOCKET clientSocket;
	sockaddr_in serverAddress;
	std::vector<SOCKET> clientsContainer;
	std::unordered_map<SOCKET, std::string> socketToUserName;
	std::mutex clientMutex;
};
