#include "../inc/Connection.hpp"
#include "../inc/Server.hpp"

namespace irc
{
	Connection::Connection(): socket_(), status_(true) { }

	Connection::Connection(int fd): socket_(fd), status_(true)	{ }
	
	Connection::~Connection() {}

	int Connection::GetFd() const
	{ return socket_.GetFd(); }

	void Connection::CloseConnection()
	{ status_ = false ;}

	bool Connection::GetStatus() const
	{ return status_; }

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
		{	int new_fd = Accept();
			Server::AddConnection(new ClientConnection(new_fd));
			std::cout << "NEW CLIENT RECEIVED" << "\n";
		}
		catch(const std::exception& error)
		{
			std::cerr << error.what() << '\n';
		}
	}

	void ClientConnection::Receive()
	{
		char buffer[512];
		ssize_t received;

		received = recv(GetFd(), buffer, sizeof(buffer), 0);
		if (received == -1)
			return ;
		buffer_.Append(buffer);
		std::cout << "input received: " << buffer << std::endl;
	}

	void ServerConnection::Send() {}

	ClientConnection::ClientConnection(int fd): Connection(fd) { }

	ClientConnection::~ClientConnection() { }

	void ClientConnection::Send() {}

}