#include "net/Request.h"
#include "net/Server.h"

#include <iostream>

int main()
{
	Server* server = new Server("127.0.0.1", "80");
	
	return 0;
}