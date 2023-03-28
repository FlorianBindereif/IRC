#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <map>
#include <poll.h>
#include "config.hpp"
#include "Connection.hpp"
#include "Channel.hpp"

namespace irc
{
	class Server
	{
		private:
			typedef std::vector<Connection *>::iterator			 			conn_iter;
			typedef std::vector<pollfd>::iterator				 			poll_iter;
			typedef std::map<std::string, Channel>::iterator				chan_iter;
			typedef std::vector<pollfd>::size_type							size_type;

		private:
			static std::vector<pollfd> 										polls_;
			static std::vector<Connection *> 								connections_;
			static std::map<std::string, Channel> 							channels_;
			static std::string												server_password_;

		private:
						Server(const Server& other);
		public:
						Server();
						~Server();
		private:	
			void		CleanUp_();
		public:
			void		Init(std::string password = PASSWORD, int port = PORT);
			void		Run();

			static void					AddConnection(ClientConnection* new_connection);
			static ClientConnection* 	GetConnection(std::string& nick);
			static Channel* 			GetChannel(std::string& channel_name);
			static void				AddChannel(std::string& channel_name);
			static bool 				AuthenticatePassword(std::string& password);
			static bool					CheckNickAvailability(std::string& nick);

	};
}

#endif