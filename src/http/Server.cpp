#include "Server.h"
#include "Request.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>

Server::Server(std::string ip_addr, std::string port)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) throw;

	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo("192.168.0.108", "80", &hints, &addr) != 0)
	{
		WSACleanup();
		std::terminate();
	}

	listen_socket = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
	if (listen_socket == INVALID_SOCKET)
	{
		freeaddrinfo(addr);
		WSACleanup();
		std::terminate();
	}

	if (bind(listen_socket, addr->ai_addr, (int)addr->ai_addrlen) == SOCKET_ERROR)
	{
		freeaddrinfo(addr);
		closesocket(listen_socket);
		WSACleanup();
		std::terminate();
	}

	if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		freeaddrinfo(addr);
		closesocket(listen_socket);
		WSACleanup();
		std::terminate();
	}

	std::cout << "Server initialized" << std::endl;
}

Server::~Server()
{
	closesocket(listen_socket);
	freeaddrinfo(addr);
	WSACleanup();
}

void Server::run()
{
	for (;;)
	{
		int client_socket = accept(listen_socket, NULL, NULL);
		SOCKADDR_IN client_info = { 0 };
		int addrsize = sizeof(client_info);
		const int max_client_buffer_size = 1024;
		char buf[max_client_buffer_size];

		if (client_socket == INVALID_SOCKET) {
			std::cerr << "accept failed: " << WSAGetLastError() << "\n";
			closesocket(listen_socket);
			WSACleanup();
			std::terminate();
		}

		int result = recv(client_socket, buf, max_client_buffer_size, 0);

		char ip[32];
		inet_ntop(AF_INET, (const void*)&client_info.sin_addr, ip, 32);
		std::cout << "Server get request from\t" << ip << std::endl;

		std::stringstream response;
		std::stringstream response_body;

		if (result == SOCKET_ERROR)
		{
			std::cerr << "recv failed: " << result << "\n";
			closesocket(client_socket);
		}
		else if (result == 0)
		{
			std::cerr << "connection closed...\n";
		}
		else if (result > 0)
		{
			buf[result] = '\0';
			Request request = parseRequest(buf);

			if (request.method == Request::Method::Get)
			{
				request.url == "/" ? request.url = "/index.html" : request.url;
				std::cout << request.url << std::endl;
				if (files_.contains(request.url))
				{
					response_body << files_[request.url].data;
					response << "HTTP/1.1 200 OK\r\n"
						<< "Version: HTTP/1.1\r\n"
						<< "Content-Type: " << contentTypeToString(files_[request.url].type) << "; charset=utf-8\r\n"
						<< "Content-Length: " << response_body.str().length()
						<< "\r\n\r\n"
						<< response_body.str();
				}
				else
				{
					response << "HTTP/1.1 404 NOTFOUND\r\n"
						<< "Version: HTTP/1.1\r\n";
				}
			}
			result = send(client_socket, response.str().c_str(),
				response.str().length(), 0);

			if (result == SOCKET_ERROR)
			{
				std::cerr << "send failed: " << WSAGetLastError() << "\n";
			}

			closesocket(client_socket);
		}
	}
}

void Server::setFrontendDirectory(std::string path)
{
	for (auto& dp : std::filesystem::recursive_directory_iterator(path))
	{
		if (dp.is_regular_file())
		{
			std::string path = dp.path().string();
			std::replace(path.begin(), path.end(), '\\', '/');
			std::string resourceName = path;
			resourceName.erase(path.find("frontend"), path.find("frontend") + (sizeof("frontend") * sizeof(char)) - 1);
			
			std::ifstream in;
			std::stringstream ssin;
			in.open(path, std::ios::out | std::ios::binary);
			ssin << in.rdbuf();

			ContentFile file;
			file.type = extentionToContentType(dp.path().extension().string());
			file.data = ssin.str();
			files_.insert({ resourceName,  file});
		}
	}
}