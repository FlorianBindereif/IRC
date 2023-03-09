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

	int Server::Init(std::string password, int port)
	{
		ServerConnection* server_con = new ServerConnection();
		server_con->SetOptions();
		server_con->Bind(port);
		server_con->Listen();
		connections_.push_back(server_con);

		pollfd server_poll = {.revents = 0, .events = POLLEVENTS, .fd = server_con->GetFd()};
		polls_.push_back(server_poll);

		password_ = password;
	}

	void Server::Run()
	{
		int revent_count;
		
		revent_count = poll(polls_.data(), polls_.size(), 100);
		if (revent_count == -1)
			return ;
		for (int i = 0; i < polls_.size(); i++)
		{
			if (polls_[i].revents == 0)
				continue;
			if (polls_[i].revents & (POLLERR | POLLHUP | POLLNVAL))
				connections_[i]->CloseConnection();
			else if (polls_[i].revents & POLL_IN)
				connections_[i]->Receive();
			if (polls_[i].revents & POLL_OUT)
				connections_[i]->Send();
		}
		CleanUp();
	}

	void Server::CleanUp()
	{
		//delete aus usermap noch notwendig
		int i = 0;
		while (i < connections_.size())
		{
			if (connections_[i]->GetStatus() == false)
			{
				connections_.erase(connections_.begin() + i);
				polls_.erase(polls_.begin() + i);
				continue;
			}
			i++;
		}
	}

	void Server::AddConnection(ClientConnection* new_connection)
	{
		Server::connections_.push_back(new_connection);
		pollfd client_poll = {.revents = 0, .events = POLLEVENTS, .fd = new_connection->GetFd()};
		Server::polls_.push_back(client_poll);
	}
}