#include <iostream>
#include "../inc/Server.hpp"

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		std::cout << "Too few arguments: ./irc [port > 0] [password]" << std::endl;
		return 0;
	}
	int port = atoi(argv[1]);
	if (port <= 0)
	{
		std::cout << "bad port: ./irc [port > 0] [password]" << std::endl;
		return 0;
	}
	try
	{
		irc::Server server;
		server.Init();
		while(true)
		{
			server.Run();
		}
	}
	catch(const std::exception& error)
	{
		std::cerr << error.what() << '\n';
	}
}