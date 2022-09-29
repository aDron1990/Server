#include "http/Server.h"

#include <clocale>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	setlocale(LC_ALL, "Russian");

	Server* server = new Server("192.168.0.108", "80");
	server->setFrontendDirectory("frontend");
	server->run();
	delete server;

	return 0;
}