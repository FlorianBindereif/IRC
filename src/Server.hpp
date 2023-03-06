#pragma once
#include <vector>
#include "Connection.hpp"
#include <map>
#include <poll.h>

namespace irc
{
	class Server
	{
		private:
			std::vector<Connection *>::iterator				conn_iter;
			std::vector<pollfd>::iterator					poll_iter;
			std::map<std::string, Connection *>::iterator	nick_iter;
		private:
			std::vector<pollfd> polls_;
			std::vector<Connection *> connections_;
			std::map<std::string, Connection *> nicks_;
			std::string password_;
		private:
			Server(const Server& other);
		public:
			Server();
			~Server();
		private:
			// void ResetFlags(); kann auch für jede connection nach dem pollen resetted werden
		public:
			int		Init(std::string password, int port);
			void		Run();
			void	AddUser();
			void	RemoveUser();
	};
}