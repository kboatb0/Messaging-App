#include "client.h"

int main()
{
    WSADATA wsadata;
    int initStat = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (initStat != 0) {
        std::cerr << "WSAStartup failed with code: " << initStat << '\n';
        return 1;
    }

    Client client;

    std::thread sendThread([&client]() {
        client.sendMessage();
        });

    std::thread receiveThread([&client]() {
        client.receiveMessage();
        });
}