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

	int Server::Init(std::string password = PASSWORD, int port = PORT)
	{
		ServerConnection* server_con = new ServerConnection;
		server_con->Bind(port);
		server_con->Listen();
		connections_.push_back(server_con);

		pollfd server_poll{.revents = 0, .events = POLLEVENTS, .fd = server_con->GetFd()};
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
			if (polls_[i].revents == 0)
				continue;
			else if (polls_[i].revents & POLLERR | POLLHUP | POLLNVAL)
				connections_[i]->CloseConnection();
			else if (polls_[i].revents & POLL_IN)
				connections_[i]->Receive();
			if (polls_[i].revents & POLL_OUT)
				connections_[i]->Send();
			polls_[i].revents = 0;
		}
	}
}