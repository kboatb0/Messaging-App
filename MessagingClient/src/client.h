#pragma once

#include <iostream>
#include <winsock2.h>
#include <thread>
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