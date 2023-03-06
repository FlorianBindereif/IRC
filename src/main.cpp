#include <iostream>
#include "Server.hpp"

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		// proper error management still required
		std::cout << "Too few arguments: ./irc [port > 0] [password]" << std::endl;
		return 0;
	}
	int port = atoi(argv[1]);
	if (port <= 0)
	{
		// proper error management still required
		std::cout << "bad port: ./irc [port > 0] [password]" << std::endl;
		return 0;
	}
	// try catch block required still
	irc::Server server();
	server.Init()
	while(true)
	{
		server.Run();
	}
}