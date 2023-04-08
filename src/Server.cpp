#include "../inc/Server.hpp"
#include "../inc/Print.hpp"

namespace irc
{
	std::vector<Connection *> Server::connections_;
	std::vector<pollfd> Server::polls_;
	std::map<std::string, Channel> Server::channels_;
	std::string Server::server_password_;

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

	void Server::Init(std::string password, int port)
	{
		ServerConnection* server_con = new ServerConnection();
		server_con->SetOptions();
		server_con->Bind(port);
		server_con->Listen();
		connections_.push_back(server_con);

		pollfd server_poll = {.revents = 0, .events = POLLEVENTS, .fd = server_con->GetFd()};
		polls_.push_back(server_poll);

		server_password_ = password;
	}

	void Server::Run()
	{
		int revent_count = poll(polls_.data(), polls_.size(), 100);
		if (revent_count == -1)
			return ;
		for (size_type i = 0; i < polls_.size(); i++)
		{
			if (polls_[i].revents == 0)
				continue;
			if (polls_[i].revents & (POLLERR | POLLHUP | POLLNVAL))
				connections_[i]->CloseConnection();
			else if (polls_[i].revents & POLLIN)
				connections_[i]->Receive();
			if (polls_[i].revents & POLLOUT)
				connections_[i]->Send();
		}
		CleanUp_();
	}

	void Server::CleanUp_()
	{
		size_type i = 0;

		while (i < connections_.size())
		{
			polls_[i].revents = 0;
			if (connections_[i]->GetStatus() == DISCONNECTED)
			{
				delete connections_[i];
				connections_.erase(connections_.begin() + i);
				polls_.erase(polls_.begin() + i);
			}
			else
				i++;
		}
		for (chan_iter it = channels_.begin(); it != channels_.end();)
		{
			if (it->second.IsEmpty())
				it = channels_.erase(it);
			else
				it++;
		}
	}

	void Server::AddConnection(ClientConnection* new_connection)
	{
		Server::connections_.push_back(new_connection);
		pollfd client_poll = {.revents = 0, .events = POLLEVENTS, .fd = new_connection->GetFd()};
		Server::polls_.push_back(client_poll);
	}

	Channel* Server::AddChannel(std::string& channel_name)
	{ 
		std::pair<std::string, Channel> pair = std::make_pair(channel_name, Channel(channel_name));
		return &(channels_.insert(pair).first->second);
	}

	Channel* Server::GetChannel(std::string& channel_name)
	{
		chan_iter find = channels_.find(channel_name);
		return find == channels_.end() ? nullptr : &(find->second);
	}

	bool Server::AuthenticatePassword(std::string& password)
	{ return server_password_ == password ? true : false; }

	bool Server::CheckNickAvailability(std::string& nick)
	{
		for (conn_iter it = connections_.begin(); it != connections_.end(); it++)
		{
			if ((*it)->user.nick == nick)
				return false;
		}
		return true;
	}

	ClientConnection* Server::GetConnection(std::string& nick)
	{
		for (conn_iter it = connections_.begin(); it != connections_.end(); it++)
		{
			if ((*it)->user.nick == nick)
				return dynamic_cast<ClientConnection *>(*it);
		}
		return nullptr;
	}
}