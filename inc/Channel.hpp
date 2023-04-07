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
#define CHANINVITE 0b0010
#define CHANTOPIC 0b0100
#define CHANBOT 0b1000

namespace irc
{
	class Channel
	{
		private:
			unsigned char								mode_;
			std::string 								name_;
			std::string									topic_;
			std::vector<ClientConnection*>				invited_;			
			std::map<ClientConnection *, unsigned char>	registered_;
		public:
			Channel(std::string name);
		public:
			std::string GetName() const;
			unsigned char GetMode() const;
			std::string GetModeString() const;
			bool IsEmpty();
			bool IsOperator(ClientConnection* connection);
			bool IsInvis(ClientConnection* connection);
			bool IsInvited(ClientConnection* connection);
			bool IsRegistered(ClientConnection* connection);
			void Broadcast(const std::string& message, std::string exlude_nick = std::string());
			void AddConnection(ClientConnection* to_join, unsigned char permissions);
			void RemoveConnection(ClientConnection* connection);
			void GiveInvite(ClientConnection* connection);
			void TakeInvite(ClientConnection* connection);
			void GiveOperator(ClientConnection* connection);
			void TakeOperator(ClientConnection* connection);
			void GiveInvis(ClientConnection* connection);
			void TakeInvis(ClientConnection* connection);
			void SetTopic(std::string& topic);
			std::string GetTopic();
			void SetChannelMode(std::string mode);
			void SetRegisteredMode(ClientConnection* target, std::string& mode);
			std::string GetRegisteredString(bool include_invis = true);
	};
}

#endif