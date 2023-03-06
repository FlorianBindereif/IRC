#include "Server.hpp"
#include "../config.hpp"

namespace irc
{
	Server::Server()
	{ 
		polls_.reserve(1024);
		connections_.reserve(1024);
	}

	Server::~Server()
	{
		for (conn_iter it = connections_.begin(); it != connections_.end(); it++)
			delete(*it);
	}

	int Server::Init(std::string password, int port = PORT)
	{
		ServerConnection* server_con = new ServerConnection;
		server_con->Bind(port);
		server_con->Listen();
		connections_.push_back(server_con);

		pollfd server_poll{.revents = 0, .events = POLLIN | POLLHUP, .fd = server_con->GetFd()};
		polls_.push_back(server_poll);

		password_ = password;
	}

	void Server::Run()
	{
		int revent_count;
		revent_count = poll(polls_.data(), polls_.size(), -1);

		if (revent_count == -1)
			return ;
		for (int i = 0; i < polls_.size(); i++)
		{
			
		}
	}
}