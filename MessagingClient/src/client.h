#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <thread>
#include <mutex>
#include <string>

class Client {
public:
	Client();
	~Client();
	void connectToServer();
	void sendMessage();
	void receiveMessage();
private:
	SOCKET clientSocket;
	sockaddr_in serverAddress;
};