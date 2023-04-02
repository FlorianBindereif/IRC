#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>
#include "Connection.hpp"

namespace irc
{
	class Channel
	{
		public:
			enum Permission
			{
				USER,
				OPERATOR,
				NOTREGISTERED
			};

		private:
			unsigned char								mode_;
			std::string 								name_;
			std::map<ClientConnection *, Permission>	registered_;
		public:
			Channel(std::string name);
		public:
			std::string GetName() const;
			unsigned char GetMode() const;
			std::string GetModeString() const;
			void Broadcast(const std::string& message);
			void AddConnection(ClientConnection* to_join, Permission permission);	
			void RemoveConnection(ClientConnection* connection);
			Permission GetPermissions(ClientConnection* connection);
			void GiveOperatorPermission(ClientConnection* connection);
			void TakeOperatorPermission(ClientConnection* connection);
			void SetChannelMode(std::string mode);
			void SetRegisteredMode(ClientConnection* target, std::string& mode);
			std::string GetRegisteredString() const;
	};
}

#endif