#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <map>
#include "Connection.hpp"

namespace irc
{
	class Channel
	{
		private:
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
			void AddConnection(ClientConnection* connection);
			void RemoveConnection(ClientConnection* connection);
			void GiveOperatorPermission(ClientConnection* connection);
			void TakeOperatorPermission(ClientConnection* connection);
	};
}

#endif