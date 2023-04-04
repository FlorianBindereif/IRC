#include "../inc/Connection.hpp"
#include "../inc/Parser.hpp"
#include "../inc/Server.hpp"
#include "../inc/Print.hpp"
#include "../inc/ServerReponse.hpp"
#include "../inc/config.hpp"
#include "../inc/Utils.hpp"
#include "../inc/Channel.hpp"

#include <iostream>
#include <string>
#include <unistd.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>

namespace irc
{
	Connection::Connection(): socket_(), state(CONNECTED) {}

	Connection::Connection(int fd): socket_(fd), state(CONNECTED){ }
	
	Connection::~Connection() {}

	int Connection::GetFd() const
	{ return socket_.GetFd(); }

	void Connection::CloseConnection()
	{ state = DISCONNECTED ;}

	ConnectionState& Connection::GetStatus()
	{ return state; }

	/* Sets Socket into listening mode for @BACKLOG elements in queue */
	void ServerConnection::Listen()
	{
		if (listen(GetFd(), BACKLOG) == -1)
			throw std::runtime_error("listen(GetFd(), BACKLOG) returned ERROR");
	}

	ServerConnection::ServerConnection() {};

	ServerConnection::~ServerConnection() {};

	/* Binds ServerSocket to @port */
	void ServerConnection::Bind(int port)
	{
		struct sockaddr_in server_address;

		memset(&server_address, 0, sizeof(server_address));
		server_address.sin_family = AF_INET;
		server_address.sin_addr.s_addr = inet_addr(IP);
		server_address.sin_port = htons(port);

		if (bind(GetFd(), (struct sockaddr *) &server_address, sizeof(server_address)) == -1)
			throw std::runtime_error("bind(fd, ...) returned ERROR");
	}

	/* Sets socket options at socket level (SOL_SOCKET) 
	which allows the doublicate address and port bindings (SO_REUSEPORT)*/
	void ServerConnection::SetOptions()
	{
		int value = 1;
		if (setsockopt(GetFd(), SOL_SOCKET, SO_REUSEPORT, &value, sizeof(value)) == -1)
			throw std::runtime_error("setsockopt(fd, SOL_SOCKET, ...) returned ERROR");
	}

	/* Accepts an incoming request for a new client Connection */
	int	ServerConnection::Accept()
	{
		struct sockaddr_in client_address;
		socklen_t size = sizeof(client_address);
		int new_fd = accept(GetFd(), (sockaddr *) &client_address, &size);
		if (new_fd == -1)
			throw std::runtime_error("accept(GetFd(), ...) returned ERROR");
		return new_fd;
	}

	/* Adds a new Client Connection to the Server */
	void ServerConnection::Receive()
	{
		try
		{
			int new_fd = Accept();
			Server::AddConnection(new ClientConnection(new_fd));
		}
		catch(const std::exception& error)
		{ std::cerr << error.what() << '\n'; }
	}

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

	void ClientConnection::ExecuteMessage(std::string command)
	{
		//parser still needs error management
		// if (command.length() > MESSAGE_LENGTH) {}
		Message message;

		message = MessageParser::Parse(command);
		std::cout << message << std::endl;

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
	}

	std::string ClientConnection::GetModeString_() const
	{
		std::string mode("+");
		if (mode_ & 0b01)
			mode += "i";
		if (mode_ & 0b10)
			mode += "o";
		return mode; 
	}

	void ClientConnection::SetClientMode_(Message& message)
	{
		ClientConnection *client = Server::GetConnection(message.middle_params[0]);
		if (client == nullptr)
			return output_buffer_.Append(ERR_NOSUCHNICK(message.middle_params[0]));
		if (client->user.nick != user.nick)
			return output_buffer_.Append(ERR_USERSDONTMATCH(user.nick, message.middle_params[0]));
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
				else if (mode[i] == '+')
					add = true;
				else if (mode[i] == 'i' && add)
					mode_ |= 0b01;
				else if (mode[i] == 'i' && !add)
					mode_ &= 0b10;
				else if (mode[i] == 'o' && add)
					mode_ |= 0b10;
				else if (mode[i] == 'o' && !add)
					mode_ &= 0b01;
			}
		}
		output_buffer_.Append(RPL_MODEUSER(user.nick, GetModeString_()));
	}

	void ClientConnection::SetChannelMode_(Message& message)
	{
		Channel* channel = Server::GetChannel(message.middle_params[0]);
		if (channel == nullptr)
			return output_buffer_.Append(ERR_NOSUCHCHANNEL(user.nick, message.middle_params[0]));
		if (message.middle_params.size() == 1)
			return output_buffer_.Append(RPL_CHANNELMODEIS(user.nick, message.middle_params[0], channel->GetModeString()));
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

	void ClientConnection::SetMode(Message& message)
	{
		if (message.middle_params[0].empty())
			return output_buffer_.Append(ERR_NEEDMOREPARAMS(message.command));
		if (message.middle_params[0].front() == '#')
			SetChannelMode_(message);
		else
			SetClientMode_(message);
	}

	void ClientConnection::SendPong(Message& message)
	{ output_buffer_.Append(RPL_PING(user.nick, message.middle_params[0]));	}

	void ClientConnection::JoinChannel(Message& message)
	{
		if (message.middle_params.empty())
			return output_buffer_.Append(ERR_NEEDMOREPARAMS(message.command));
		std::istringstream iss(message.middle_params[0]);
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
				else
					channel->AddConnection(this, REGISTERED);
				channel_list.push_back(channel->GetName());
				channel->Broadcast(RPL_JOIN(user.nick, user.username, channel_name));
				output_buffer_.Append(RPL_NAMREPLY(user.nick, channel->GetName(), channel->GetRegisteredString()));
				output_buffer_.Append(RPL_ENDOFNAMES(user.nick, channel->GetName()));
			}
			//hier muss dan invite hin;
			//topic muss dann heir noch hin;
		}
	}

	bool ClientConnection::IsInChannel_(const std::string& channel_name)
	{
		for (std::vector<std::string>::size_type i = 0; i < channel_list.size(); i++)
		{
			if (channel_list[i] == channel_name)
				return true;
		}
		return false;
	}

	void ClientConnection::SendNames(Message& message)
	{
		if (message.middle_params.empty())
			return output_buffer_.Append(ERR_NEEDMOREPARAMS(message.command));
		std::istringstream iss(message.middle_params[0]);
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
			if (IsInChannel_(channel->GetName()))
				reply = channel->GetRegisteredString();
			else
				reply = channel->GetRegisteredString(false);
			if (!reply.empty())
				output_buffer_.Append(RPL_NAMREPLY(user.nick, channel_name, reply));
			output_buffer_.Append(RPL_ENDOFNAMES(user.nick, channel_name));
		}
	}

	void ClientConnection::Authenticate(Message& message)
	{
		if (state == AUTHENTICATED || state == REGISTERED)
			return output_buffer_.Append(ERR_ALREADYREGISTRED());
		if (message.middle_params[0].empty())
			return output_buffer_.Append(ERR_NEEDMOREPARAMS(message.command));
		if (!Server::AuthenticatePassword(message.middle_params[0]))
			return output_buffer_.Append(ERR_PASSWDMISMATCH());
		state = AUTHENTICATED;
	}

	void ClientConnection::SendCapabilities(Message& message)
	{ 
		(void) message;
		// output_buffer_.Append(RPL_CAP());
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
		if (message.middle_params[0].empty())
			return output_buffer_.Append(ERR_NONICKNAMEGIVEN());
		if (Server::CheckNickAvailability(message.middle_params[0]))
		{
			if (state == REGISTERED)
			{
				for (std::vector<std::string>::size_type i = 0; i < channel_list.size(); i++)
				{
					Channel* channel = Server::GetChannel(channel_list[i]);
					if (channel != nullptr)
						channel->Broadcast(RPL_NICKCHANGE(user.nick, message.middle_params[0], user.username));
				}
			}
			else if (!user.username.empty())
			{
				state = REGISTERED;
				output_buffer_.Append(RPL_WELCOME(message.middle_params[0], user.username));
			}
			user.nick = message.middle_params[0];
		}
		else
			output_buffer_.Append(ERR_NICKNAMEINUSE(message.middle_params[0]));
	}

	void ServerConnection::Send() {}

	ClientConnection::ClientConnection(int fd): Connection(fd), mode_(0) { }

	ClientConnection::~ClientConnection() { }

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
}