#include "Server.hpp"

namespace irc
{
	namespace server
	{
		Server::Server()
		{ 
			polls_.reserve(1024);
			conns_.reserve(1024);
		}

		Server::Init()
		{
			connection::ServerConnection server_con = new connection::ServerConnection;
			polls_.push_back
		}
	}
}