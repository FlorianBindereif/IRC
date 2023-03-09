#include "../inc/Connection.hpp"
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../inc/config.hpp"
#include "../inc/Server.hpp"

namespace irc
{
	int Connection::GetFd() const
	{ return socket_.GetFd(); }

	/* Sets Socket into listening mode for @BACKLOG elements in queue */
	void ServerConnection::Listen()
	{
		if (listen(GetFd(), BACKLOG) == -1)
			throw std::runtime_error("listen(GetFd(), BACKLOG) returned ERROR");
	}

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

	Connection::Connection(int fd): socket_(fd)
	{ }
	ClientConnection::ClientConnection(int fd): Connection(fd)
	{ }

	/* Accepts an incoming request for a new client Connection */
	void ServerConnection::Receive()
	{
		try
		{
			struct sockaddr_in client_address;
			socklen_t size = sizeof(client_address);
			int new_socket = accept(GetFd(), (sockaddr *) &client_address, &size);
			if (new_socket == -1)
				throw std::runtime_error("accept(GetFd(), ...) returned ERROR");
			ClientConnection* new_client =  new ClientConnection(new_socket);
			Server::AddConnection();
		}
		catch(const std::exception& error)
		{
			std::cerr << error.what() << '\n';
		}
	}
}