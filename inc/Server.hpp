#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include "config.hpp"
#include <map>
#include <poll.h>
#include "Connection.hpp"

namespace irc
{
	class Server
	{
		private:
			typedef std::vector<Connection *>::iterator			 	conn_iter;
			typedef std::vector<pollfd>::iterator				 	poll_iter;
			typedef std::map<std::string, Connection *>::iterator 	nick_iter;
			typedef std::vector<pollfd>::size_type					size_type;

		private:
			static std::vector<pollfd> polls_;
			static std::vector<Connection *> connections_;
			static std::map<std::string, Connection *> nicks_;
			std::string password_;

		private:
			Server(const Server& other);
		public:
			Server();
			~Server();
		public:
			void	Init(std::string password = PASSWORD, int port = PORT);
			void	Run();
			void	CleanUp();
			static void	AddConnection(ClientConnection* new_connection);
	};
}

#endif