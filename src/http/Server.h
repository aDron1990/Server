#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <iostream>
#include <map>

#include "ContentFile.h"

class Server
{
private:

	int listen_socket;
	addrinfo* addr;
	addrinfo hints;

	std::map<std::string, ContentFile> files_;

public:

	Server(std::string ip_addr, std::string port);
	~Server();

	void run();
	void setFrontendDirectory(std::string path);
};