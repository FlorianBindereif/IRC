#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <map>
#include "Connection.hpp"

namespace irc
{
	class Channel
	{
		enum Permission
		{
			USER,
			OPERATOR
		};

		private:
			std::string 								name_;
			std::map<ClientConnection *, Permission>	connections_;
		public:
			Channel(std::string name);
		public:
			void Broadcast(std::string& message) const;
			void AddConnection(ClientConnection* to_join);
			void RemoveConnection(ClientConnection* to_leave);
	};
}

#endif