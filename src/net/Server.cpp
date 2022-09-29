#include "Server.h"

#include <sstream>

Server::Server(std::string ip_addr, std::string port)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		std::cerr << "Failed to startup WSA: " << WSAGetLastError() << std::endl;
		std::terminate();
	}

	addrinfo* addr = 0;
	addrinfo hints = {0};

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo("127.0.0.1", "80", &hints, &addr))
	{
		std::cerr << "Address initialize failed: " << WSAGetLastError() << std::endl;
		WSACleanup();
		std::terminate();
	}

	listen_socket = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
	if (listen_socket == INVALID_SOCKET)
	{
		std::cerr << "Listen socket initialize error: " << WSAGetLastError() << std::endl;
		freeaddrinfo(addr);
		WSACleanup();
		std::terminate();
	}

	if (bind(listen_socket, addr->ai_addr, addr->ai_addrlen) == SOCKET_ERROR)
	{
		std::cerr << "Socket bind failed: " << WSAGetLastError() << std::endl;
		freeaddrinfo(addr);
		closesocket(listen_socket);
		WSACleanup();
		std::terminate();
	}

	if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR) 
	{
		std::cerr << "listen failed with error: " << WSAGetLastError() << "\n";
		closesocket(listen_socket);
		WSACleanup();
		std::terminate();
	}

	int client_socket = accept(listen_socket, NULL, NULL);
	if (client_socket == INVALID_SOCKET) {
		std::cerr << "accept failed: " << WSAGetLastError() << "\n";
		closesocket(listen_socket);
		WSACleanup();
		std::terminate();
	}

	const int max_client_buffer_size = 1024;
	char buf[max_client_buffer_size];

	int result = recv(client_socket, buf, max_client_buffer_size, 0);

	std::stringstream response; // сюда будет записываться ответ клиенту
	std::stringstream response_body; // тело ответа

	if (result == SOCKET_ERROR) {
		// ошибка получения данных
		std::cerr << "recv failed: " << result << "\n";
		closesocket(client_socket);
	}
	else if (result == 0) {
		// соединение закрыто клиентом
		std::cerr << "connection closed...\n";
	}
	else if (result > 0) {
		// Мы знаем фактический размер полученных данных, поэтому ставим метку конца строки
		// В буфере запроса.
		buf[result] = '\0';

		// Данные успешно получены
		// формируем тело ответа (HTML)
		response_body << "<title>Test C++ HTTP Server</title>\n"
			<< "<h1>Test page</h1>\n"
			<< "<p>This is body of the test page...</p>\n"
			<< "<h2>Request headers</h2>\n"
			<< "<pre>" << buf << "</pre>\n"
			<< "<em><small>Test C++ Http Server</small></em>\n";

		// Формируем весь ответ вместе с заголовками
		response << "HTTP/1.1 200 OK\r\n"
			<< "Version: HTTP/1.1\r\n"
			<< "Content-Type: text/html; charset=utf-8\r\n"
			<< "Content-Length: " << response_body.str().length()
			<< "\r\n\r\n"
			<< response_body.str();

		// Отправляем ответ клиенту с помощью функции send
		result = send(client_socket, response.str().c_str(),
			response.str().length(), 0);

		if (result == SOCKET_ERROR) {
			// произошла ошибка при отправле данных
			std::cerr << "send failed: " << WSAGetLastError() << "\n";
		}
		// Закрываем соединение к клиентом
		closesocket(client_socket);
	}

	closesocket(listen_socket);
	freeaddrinfo(addr);
	WSACleanup();
}