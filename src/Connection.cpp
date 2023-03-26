#include "../inc/Connection.hpp"
#include "../inc/Parser.hpp"
#include "../inc/Server.hpp"
#include "../inc/Print.hpp"
#include "../inc/ServerReponse.hpp"
#include "../inc/config.hpp"

#include <iostream>
#include <string>
#include <unistd.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>

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
			PRINTNL("NEW CONNECTION RECEIVED", GREEN);
		}
		catch(const std::exception& error)
		{
			std::cerr << error.what() << '\n';
		}
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
		if (message.command == "NICK")
			return SetNickname(message);
		if (message.command == "USER")
			return SetUsername(message);
		// else if (state == REGISTERED)
		// {

		// }
		// if (state == AUTHENTICATED)
		// {
		// 	if (message.command == "JOIN")
		// 		return ConnectToChannel();
		// 	if ()
		// }
	}
	
	void ClientConnection::Authenticate(Message& message)
	{
		if (state == AUTHENTICATED || state == REGISTERED)
		{
			PRINTNL("USER TRIED TO RE-AUTHENTICATE", RED);
			output_buffer_.Append(ERR_ALREADYREGISTRED());
		}
		else if (message.middle_params[0].empty())
		{
			PRINTNL("USER SENT EMPTY PASSWORD", RED);
			output_buffer_.Append(ERR_NEEDMOREPARAMS());
		}
		else if (Server::AuthenticatePassword(message.middle_params[0]))
		{
			PRINTNL("NEW USER AUTHENTICATED", GREEN);
			state = AUTHENTICATED;
		}
		else
		{
			PRINTNL("USER PROVIDED INVALID PASSWORD", RED);
			output_buffer_.Append(ERR_PASSWDMISMATCH());
		}
	}

	void ClientConnection::SendCapabilities(Message& message)
	{ 
		PRINTNL("CAPABILITIES SENT", GREEN);
		(void) message;
	}

	void ClientConnection::SetUsername(Message& message)
	{
		if (state == CONNECTED)
		{
			PRINTNL("USER PROVIDED INVALID PASSWORD", RED);
			output_buffer_.Append(ERR_PASSWDMISMATCH());
		}
		else if (message.middle_params.size() != 3 && message.trailing.size() != 1)
		{
			PRINTNL("USER SENT EMPTY USERNAME", RED);
			output_buffer_.Append(ERR_NEEDMOREPARAMS());
		}
		else if (!user.username.empty())
		{
			PRINTNL("USER TRIED RESETTING USERNAME", RED);
			output_buffer_.Append((ERR_ALREADYREGISTRED()));
		}
		else
		{
			user.username = message.trailing;
			std::cout << GREEN << "USERNAME SET: " << user.username << "\n";
			if (!user.nick.empty())
				state = REGISTERED;
		}
	}

	void ClientConnection::SetNickname(Message& message)
	{
		if (state == CONNECTED)
		{
			PRINTNL("USER PROVIDED INVALID PASSWORD", RED);
			output_buffer_.Append(ERR_PASSWDMISMATCH());
		}
		else if (message.middle_params[0].empty())
		{
			PRINTNL("USER SENT EMPTY NICK", RED);
			output_buffer_.Append(ERR_NONICKNAMEGIVEN());
		}
		else if (Server::CheckNickAvailability(message.middle_params[0]))
		{
			std::cout << GREEN << "NICKNAME " << message.middle_params[0] << " SET" << RESET << "\n";
			user.nick = message.middle_params[0];
			if (!user.username.empty())
				state = REGISTERED;
		}
		else
		{
			output_buffer_.Append(ERR_NICKNAMEINUSE(message.middle_params[0]));
			PRINTNL("USER TRIED SETTING NICK ALREADY IN USE", RED);
		}
	}

	void ServerConnection::Send() {}

	ClientConnection::ClientConnection(int fd): Connection(fd) { }

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