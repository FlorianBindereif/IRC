#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>
#include "Connection.hpp"


//Registered Modes
#define NOTMEMBER 0b0000
#define MEMBER 0b0001
#define INVIS 0b0010
#define OPERATOR 0b0100

//Channel Modes
#define CHANMOD 0b0001
#define CHANINITE 0b0010
#define CANTOPIC 0b0100
#define CANBAN 0b1000

namespace irc
{
	class Channel
	{
		private:
			unsigned char								mode_;
			std::string 								name_;
			std::map<ClientConnection *, unsigned char>	registered_;
		public:
			Channel(std::string name);
		public:
			std::string GetName() const;
			unsigned char GetMode() const;
			std::string GetModeString() const;
			bool IsOperator(ClientConnection* connection);
			bool IsInvis(ClientConnection* connection);
			void Broadcast(const std::string& message);
			void AddConnection(ClientConnection* to_join, unsigned char permissions);
			void RemoveConnection(ClientConnection* connection);
			void GiveOperator(ClientConnection* connection);
			void TakeOperator(ClientConnection* connection);
			void GiveInvis(ClientConnection* connection);
			void TakeInvis(ClientConnection* connection);
			void SetChannelMode(std::string mode);
			void SetRegisteredMode(ClientConnection* target, std::string& mode);
			std::string GetRegisteredString(bool include_invis = true);
	};
}

#endif