#pragma once
#include <vector>
#include "Connection.hpp"
#include <map>
#include <poll.h>

namespace irc
{
	namespace server
	{
		class Server
		{
			private:
				std::vector<pollfd> polls_;
				std::vector<irc::connection::Connection *> conns_;
				std::map<std::string, irc::connection::Connection *> nicks_;

			private:
				Server(const Server& other);
			
			public:
				Server();
				~Server();

			public:
				int Init();
				int Run();

				void AddUser();
				void RemoveUser();
		}
	}
}