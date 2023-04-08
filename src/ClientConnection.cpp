#include "../inc/Connection.hpp"
#include "../inc/Parser.hpp"
#include "../inc/Utils.hpp"
#include "../inc/Channel.hpp"
#include "../inc/Bot.hpp"
#include "../inc/Server.hpp"
#include "../inc/Print.hpp"
#include "../inc/ServerReponse.hpp"
#include "../inc/config.hpp"

#include <sstream>
#include <sys/socket.h>

namespace irc
{
	ClientConnection::ClientConnection(int fd): Connection(fd), mode_(0), hands(0,0) { }

	ClientConnection::~ClientConnection() { }

	void ClientConnection::Receive()
	{
		char buffer[MESSAGE_LENGTH];
		ssize_t received;

		received = recv(GetFd(), buffer, sizeof(buffer), 0);
		if (received <= 0)
			return ;
		input_buffer_.Append(buffer, 0 , received);
		while (input_buffer_.HoldsMessage())
		{ ExecuteMessage(input_buffer_.GetMessage()); }
	}

	void ClientConnection::Send()
	{
		if (output_buffer_.Empty())
			return ;
		while(output_buffer_.HoldsMessage())
		{
			std::string output = output_buffer_.GetMessageCR();
			send(GetFd(), output.data(), output.size(), 0);
		}
	}

	void ClientConnection::CloseConnection()
	{ LeaveServer(); }

	void ClientConnection::ExecuteMessage(std::string command)
	{
		Message message;
		try
		{ message = MessageParser::Parse(command); }
		catch(const std::exception& error)
		{ 
			std::cerr << "Parser error: " << error.what() << '\n';
			return ;
		}
		if (state == DISCONNECTED)
			return ;
		if (message.command == "PASS")
			return Authenticate(message);
		if (message.command == "CAP")
			return SendCapabilities(message);
		if (state < AUTHENTICATED)
			return output_buffer_.Append(ERR_NOTREGISTERED(message.command));
		if (message.command == "NICK")
			return SetNickname(message);
		if (message.command == "USER")
			return SetUsername(message);
		if (state < REGISTERED)
			return output_buffer_.Append(ERR_NOTREGISTERED(message.command));
		if (message.command == "JOIN")
			return JoinChannel(message);
		if (message.command == "PING")
			return SendPong(message);
		if (message.command == "MODE")
			return SetMode(message);
		if (message.command == "NAMES")
			return SendNames(message);
		if (message.command == "PART")
			return PartChannel(message);
		if (message.command == "PRIVMSG")
			return SendMessage(message);
		if (message.command == "INVITE")
			return InviteClient(message);
		if (message.command == "TOPIC") 
			return SetTopic(message);
		if (message.command == "KICK")
			return KickMember(message);
		if (message.command == "OPER")
			return MakeOperator(message);
		if (message.command == "NOTICE")
			return SendNotice(message);
		if (message.command == "DIE")
			return ShutdownServer(message);
		if (message.command == "QUIT")
			return LeaveServer();
		output_buffer_.Append(ERR_UNKNOWNCOMMAND(user.nick, message.command));
	}

	void ClientConnection::SendCapabilities(Message& message)
	{ 
		(void) message;
		// output_buffer_.Append(RPL_CAP());
	}

	void ClientConnection::Authenticate(Message& message)
	{
		if (state == AUTHENTICATED || state == REGISTERED)
			return output_buffer_.Append(ERR_ALREADYREGISTRED());
		if (message.middle_params.front().empty())
			return output_buffer_.Append(ERR_NEEDMOREPARAMS(message.command));
		if (!Server::AuthenticatePassword(message.middle_params.front()))
			return output_buffer_.Append(ERR_PASSWDMISMATCH());
		state = AUTHENTICATED;
	}

	void ClientConnection::SetUsername(Message& message)
	{
		if (message.middle_params.size() != 3 && message.trailing.size() != 1)
			return output_buffer_.Append(ERR_NEEDMOREPARAMS(message.command));
		if (state == REGISTERED)
			return output_buffer_.Append((ERR_ALREADYREGISTRED()));
		user.username = message.trailing;
		if(!user.nick.empty())
		{
			state = REGISTERED;
			output_buffer_.Append(RPL_WELCOME(user.nick, user.username));
		}
	}

	void ClientConnection::SetNickname(Message& message)
	{
		if (message.middle_params.front().empty())
			return output_buffer_.Append(ERR_NONICKNAMEGIVEN());
		if (!ValidNickName(message.middle_params.front()))
			return output_buffer_.Append(ERR_ERRONEUSNICKNAME(user.nick, message.middle_params.front()));
		if (!Server::CheckNickAvailability(message.middle_params.front()))
			return output_buffer_.Append(ERR_NICKNAMEINUSE(message.middle_params.front()));
		if (state == REGISTERED)
		{
			for (std::vector<std::string>::size_type i = 0; i < channel_list.size(); i++)
			{
				Channel* channel = Server::GetChannel(channel_list[i]);
				if (channel != nullptr)
					channel->Broadcast(RPL_NICKCHANGE(user.nick, message.middle_params.front(), user.username), user.nick);
			}
			output_buffer_.Append(RPL_NICKCHANGE(user.nick, message.middle_params.front(), user.username));
		}
		else if (!user.username.empty())
		{
			state = REGISTERED;
			output_buffer_.Append(RPL_WELCOME(message.middle_params.front(), user.username));
		}
		user.nick = message.middle_params.front();
	}

	void ClientConnection::JoinChannel(Message& message)
	{
		if (message.middle_params.empty())
			return output_buffer_.Append(ERR_NEEDMOREPARAMS(message.command));
		std::istringstream iss(message.middle_params.front());
		std::string channel_name;
		while(getline(iss, channel_name, ','))
		{
			if (!ValidChannelName(channel_name))
				output_buffer_.Append(ERR_NOSUCHCHANNEL(user.nick, channel_name));
			else
			{
				Channel* channel = Server::GetChannel(channel_name);
				if (channel == nullptr)
				{
					channel = Server::AddChannel(channel_name);
					channel->AddConnection(this, OPERATOR);
				}
				else if (channel->IsRegistered(this))
					continue;
				else if ((channel->GetMode() & CHANINVITE) && !channel->IsInvited(this))
				{
					output_buffer_.Append(ERR_INVITEONLYCHAN(user.nick, channel_name));
					continue ;
				}
				else
					channel->AddConnection(this, REGISTERED);
				channel_list.push_back(channel->GetName());
				channel->Broadcast(RPL_JOIN(user.nick, user.username, channel_name));
				if (!channel->GetTopic().empty())
					output_buffer_.Append(RPL_TOPIC(user.nick, channel->GetName(), channel->GetTopic()));
				output_buffer_.Append(RPL_NAMREPLY(user.nick, channel->GetName(), channel->GetRegisteredString()));
				output_buffer_.Append(RPL_ENDOFNAMES(user.nick, channel->GetName()));
			}
		}
	}

	void ClientConnection::SendPong(Message& message)
	{ output_buffer_.Append(RPL_PING(user.nick, message.middle_params.front()));	}


		void ClientConnection::SetClientMode_(Message& message)
	{
		ClientConnection *client = Server::GetConnection(message.middle_params.front());
		if (client == nullptr)
			return output_buffer_.Append(ERR_NOSUCHNICK(message.middle_params.front()));
		if (client->user.nick != user.nick)
			return output_buffer_.Append(ERR_USERSDONTMATCH(user.nick, message.middle_params.front()));
		if (message.middle_params.size() > 1)
		{
			std::string mode = CleanModeString_(message.middle_params[1], "io");
			if (mode.empty())
				return output_buffer_.Append(ERR_UMODEUNKNOWNFLAG(user.nick));
			bool add;
			for (std::string::size_type i = 0; i < mode.size(); i++)
			{
				if (mode[i] == '-')
					add = false;
				if(mode[i] == '+')
					add = true;
				if (mode[i] == 'i' && add)
					mode_ |= SERVINVIS;
				if (mode[i] == 'i' && !add)
					mode_ &= ~SERVINVIS;
				if (mode[i] == 'o' && add)
					mode_ |= SERVOP;
				if (mode[i] == 'o' && !add)
					mode_ &= ~SERVOP;
			}
		}
		output_buffer_.Append(RPL_MODEUSER(user.nick, GetModeString_()));
	}

	void ClientConnection::SetChannelMode_(Message& message)
	{
		Channel* channel = Server::GetChannel(message.middle_params.front());
		if (channel == nullptr)
			return output_buffer_.Append(ERR_NOSUCHCHANNEL(user.nick, message.middle_params.front()));
		if (message.middle_params.size() == 1)
			return output_buffer_.Append(RPL_CHANNELMODEIS(user.nick, message.middle_params.front(), channel->GetModeString()));
		if (!channel->IsOperator(this))
			return output_buffer_.Append(ERR_CHANOPRIVSNEEDED(user.nick, channel->GetName()));
		std::string mode;
		if (message.middle_params.size() == 2)
			mode = CleanModeString_(message.middle_params[1], "timb");
		else
			mode = CleanModeString_(message.middle_params[1], "io");
		if (mode.empty())
			return output_buffer_.Append(ERR_UMODEUNKNOWNFLAG(user.nick));	
		if (message.middle_params.size() == 2)
		{
			channel->SetChannelMode(mode);
			channel->Broadcast(RPL_SETMODECHANNEL(user.nick, channel->GetName(), mode));
		}
		else
		{
			ClientConnection* target = Server::GetConnection(message.middle_params[2]);
			if (target == nullptr)
				return output_buffer_.Append(ERR_NOSUCHNICK(message.middle_params[2]));
			channel->SetRegisteredMode(target, mode);
			channel->Broadcast(RPL_SETMODECLIENT(user.nick, user.username, channel->GetName(), mode, target->user.nick));
		}
	}

	std::string ClientConnection::CleanModeString_(std::string& mode, std::string flag_string)
	{	
		std::string add, remove, cleaned;

		for (std::string::size_type i = 0; i < flag_string.size(); i++)
		{
			std::string::size_type pos = mode.find_last_of(flag_string[i]);
			if (pos != std::string::npos)
			{
				std::string::size_type pos_sign = mode.find_last_of("+-", pos);
				if (pos_sign == std::string::npos)
					return cleaned;
				if (mode[pos_sign] == '+')
					add += flag_string[i];
				else
					remove += flag_string[i];
			}
		}
		if (!add.empty())
			cleaned += "+" + add;
		if (!remove.empty())
			cleaned += "-" + remove;
		return cleaned;
	}

	std::string ClientConnection::GetModeString_() const
	{
		std::string mode("+");
		if ((mode_ & SERVINVIS) == SERVINVIS)
			mode += "i";
		if ((mode_ & SERVOP) == SERVOP)
			mode += "o";
		return mode; 
	}

	void ClientConnection::SetMode(Message& message)
	{
		if (message.middle_params.front().empty())
			return output_buffer_.Append(ERR_NEEDMOREPARAMS(message.command));
		if (message.middle_params.front().front() == '#')
			SetChannelMode_(message);
		else
			SetClientMode_(message);
	}

	void ClientConnection::SendNames(Message& message)
	{
		if (message.middle_params.empty())
			return output_buffer_.Append(ERR_NEEDMOREPARAMS(message.command));
		std::istringstream iss(message.middle_params.front());
		std::string channel_name;
		while(getline(iss, channel_name, ','))
		{
			Channel* channel = Server::GetChannel(channel_name);
			if (channel == nullptr)
			{
				output_buffer_.Append(RPL_ENDOFNAMES(user.nick, channel_name));
				continue;
			}
			std::string reply;
			if (channel->IsRegistered(this))
				reply = channel->GetRegisteredString();
			else
				reply = channel->GetRegisteredString(false);
			if (!reply.empty())
				output_buffer_.Append(RPL_NAMREPLY(user.nick, channel_name, reply));
			output_buffer_.Append(RPL_ENDOFNAMES(user.nick, channel_name));
		}
	}

	void ClientConnection::PartChannel(Message& message)
	{
		if (message.middle_params.empty())
			return output_buffer_.Append(ERR_NEEDMOREPARAMS(message.command));
		std::istringstream iss(message.middle_params.front());
		std::string channel_name;
		while(getline(iss, channel_name, ','))
		{
			Channel* channel = Server::GetChannel(channel_name);
			if (channel == nullptr)
				output_buffer_.Append(ERR_NOSUCHCHANNEL(user.nick, channel_name));
			else if (!channel->IsRegistered(this))
				output_buffer_.Append(ERR_NOTONCHANNEL(user.nick, channel_name));
			else
			{
				if (message.trailing.empty())
					channel->Broadcast(RPL_PART(user.nick, user.username, channel_name));
				else
					channel->Broadcast(RPL_PART(user.nick, user.username, channel_name, message.trailing));
				std::vector<std::string>::iterator it = std::find(channel_list.begin(), channel_list.end(), channel_name);
				if (it != channel_list.end())
					channel_list.erase(it);
				channel->RemoveConnection(this);
				std::cout << channel->GetRegisteredString() << std::endl;
			}
		}
	}

	void ClientConnection::SendMessage(Message& message)
	{
		if (message.middle_params.empty() || message.trailing.empty())
			return output_buffer_.Append(ERR_NEEDMOREPARAMS(message.command));
		if (message.middle_params.front().front() == '#')
		{
			Channel* channel = Server::GetChannel(message.middle_params.front());
			if (channel == nullptr)
				return output_buffer_.Append(ERR_NOSUCHNICK(user.nick, message.middle_params.front()));
			if (!channel->IsRegistered(this))
				return output_buffer_.Append(ERR_CANNOTSENDTOCHAN(user.nick, message.middle_params.front()));
			if ((channel->GetMode() & CHANMOD) && !channel->IsOperator(this))
				return output_buffer_.Append(ERR_CHANOPRIVSNEEDED(user.nick, message.middle_params.front()));
			if ((channel->GetMode() & CHANBOT) && message.trailing.front() == '!')
				return output_buffer_.Append(RPL_BOT(user.nick, channel->GetName(), BlackJackBot(this, message)));
			return channel->Broadcast(RPL_PRIVMSG(user.nick, user.username, message.middle_params.front(), message.trailing), user.nick);
		}
		ClientConnection* target = Server::GetConnection(message.middle_params.front());
		if (target == nullptr)
			return output_buffer_.Append(ERR_NOTONCHANNEL(user.nick, message.middle_params.front()));
		target->output_buffer_.Append(RPL_PRIVMSG(user.nick, user.username, message.middle_params.front(), message.trailing));
	}

	void ClientConnection::SendNotice(Message& message)
	{
		if (message.middle_params.empty() || message.trailing.empty())
			return ;
		if (message.middle_params.front().front() == '#')
		{
			Channel* channel = Server::GetChannel(message.middle_params.front());
			if (channel == nullptr)
				return ;
			if (!channel->IsRegistered(this))
				return ;
			if ((channel->GetMode() & CHANMOD) == CHANMOD && !channel->IsOperator(this))
				return ;
			return channel->Broadcast(RPL_NOTICE(user.nick, user.username, message.middle_params.front(), message.trailing), user.nick);
		}
		Connection* target = Server::GetConnection(message.middle_params.front());
		if (target == nullptr)
			return ;
		output_buffer_.Append(RPL_NOTICE(user.nick, user.username, message.middle_params.front(), message.trailing));
	}

		void ClientConnection::ShutdownServer(Message& message)
	{
		if (mode_ & OPERATOR)
			Server::ShutDown();
		else
			output_buffer_.Append(ERR_NOPRIVS(user.nick, message.command));
	}

	void ClientConnection::LeaveServer()
	{
		for (std::vector<std::string>::size_type i = 0; i < channel_list.size(); i++)
		{
			Channel* channel = Server::GetChannel(channel_list[i]);
			if (channel != nullptr)
			{
				channel->Broadcast(RPL_QUIT(user.nick, user.username));
				channel->RemoveConnection(this);
			}
		}
		state = DISCONNECTED;
	}

	void ClientConnection::MakeOperator(Message& message)
	{
		if (message.middle_params.size() < 2)
			return output_buffer_.Append(ERR_NEEDMOREPARAMS(message.command));
		ClientConnection* target = Server::GetConnection(message.middle_params.front());
		if (target == nullptr)
			return output_buffer_.Append(ERR_NOSUCHNICK(user.nick));
		if (message.middle_params[1] != SUDOPWD)
			return output_buffer_.Append(ERR_PASSWDMISMATCH());
		target->mode_ |= OPERATOR;
		output_buffer_.Append(RPL_YOUREOPER(user.nick, target->user.nick));
	}

	void ClientConnection::KickMember(Message& message)
	{
		if (message.middle_params.size() < 2)
			return output_buffer_.Append(ERR_NEEDMOREPARAMS(message.command));
		Channel* channel = Server::GetChannel(message.middle_params.front());
		if (channel == nullptr)
			return output_buffer_.Append(ERR_NOSUCHCHANNEL(user.nick, message.middle_params.front()));
		if (!channel->IsRegistered(this))
			return output_buffer_.Append(ERR_NOTONCHANNEL(user.nick, message.middle_params.front()));
		if (!channel->IsOperator(this))
			return output_buffer_.Append(ERR_CHANOPRIVSNEEDED(user.nick, message.middle_params.front()));
		std::istringstream iss(message.middle_params[1]);
		std::string target_name;
		while(getline(iss, target_name, ','))
		{
			ClientConnection* target = Server::GetConnection(target_name);
			if (target == nullptr)
				output_buffer_.Append(ERR_NOSUCHNICK(user.nick, target_name));
			else if (!channel->IsRegistered(target))
				output_buffer_.Append(ERR_USERNOTINCHANNEL(user.nick, message.middle_params.front()));
			if (message.trailing.empty())
				channel->Broadcast(RPL_KICK(user.nick, user.username, channel->GetName(), target->user.nick));
			else
				channel->Broadcast(RPL_KICK(user.nick, user.username, channel->GetName(), target->user.nick, message.trailing));
			target->channel_list.erase(std::find(target->channel_list.begin(), target->channel_list.end(), channel->GetName()));
			channel->RemoveConnection(target);
			channel->TakeInvite(target);
		}
	}

	void ClientConnection::SetTopic(Message& message)
	{
		if (message.middle_params.empty())
			return output_buffer_.Append(ERR_NEEDMOREPARAMS(message.command));
		Channel* channel = Server::GetChannel(message.middle_params.front());
		if (channel == nullptr)
			return output_buffer_.Append(ERR_NOSUCHCHANNEL(user.nick, message.middle_params.front()));
		if (message.trailing.empty())
		{
			if (channel->GetTopic().empty())
				return output_buffer_.Append(RPL_NOTOPIC(user.nick, message.middle_params.front()));
			return output_buffer_.Append(RPL_TOPIC(user.nick, message.middle_params.front(), channel->GetTopic()));
		}
		if ((CHANTOPIC | CHANMOD) & channel->GetMode() && !channel->IsOperator(this))
			return output_buffer_.Append(ERR_CHANOPRIVSNEEDED(user.nick, message.middle_params.front()));
		channel->SetTopic(message.trailing);
		channel->Broadcast(RPL_TOPICCHANGE(user.nick, user.username, message.middle_params.front(), message.trailing));
	}

	void ClientConnection::InviteClient(Message& message)
	{
		if (message.middle_params.size() < 2)
			return output_buffer_.Append(ERR_NEEDMOREPARAMS(message.command));
		Channel* channel = Server::GetChannel(message.middle_params[1]);
		if (channel == nullptr)
			return output_buffer_.Append(ERR_NOSUCHCHANNEL(user.nick, message.middle_params[1]));
		if (!channel->IsRegistered(this))
			return output_buffer_.Append(ERR_NOTONCHANNEL(user.nick, message.middle_params[1]));
		if (!channel->IsOperator(this))
			return output_buffer_.Append(ERR_CHANOPRIVSNEEDED(user.nick, message.middle_params[1]));
		ClientConnection* target = Server::GetConnection(message.middle_params.front());
		if (target == nullptr)
			return output_buffer_.Append(ERR_NOSUCHNICK(user.nick, message.middle_params[1]));
		if (channel->IsRegistered(target))
			return output_buffer_.Append(ERR_USERONCHANNEL(user.nick, message.middle_params[1], target->user.nick));
		output_buffer_.Append(RPL_INVITING(user.nick, message.middle_params[1], target->user.nick));
		target->output_buffer_.Append(RPL_INVITED(user.nick, user.username, message.middle_params[1],target->user.nick));
		channel->GiveInvite(target);
	}
}