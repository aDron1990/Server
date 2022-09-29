#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

class Server
{
private:

	int listen_socket;

public:

	Server(std::string ip_addr, std::string port);
	~Server();

	void run();
};