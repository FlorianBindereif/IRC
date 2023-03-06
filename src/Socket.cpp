#include "Socket.hpp"
#include <sys/socket.h>
#include <fcntl.h>
#include <stdexcept>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include "../config.hpp"

namespace irc
{
	/* Constructs a new stream socket base element*/
	Socket::Socket()
	{
		fd = socket(AF_INET, SOCK_STREAM, 0);
		if (fd == -1)
			throw std::runtime_error("socket(PF_INET, SOCKSTREAM, 0) returned ERROR");
		try
		{
			SetOptions_();
			SetNonBlocking_();
		}
		catch(const std::exception& error)
		{
			Close();
			throw;
		}
	}

	/* Destructs the Socket object and closes the internal file descriptor*/
	Socket::~Socket()
	{ Close(); }
	
	/*Sets socket to non-blocking state*/
	void Socket::SetNonBlocking_()
	{
		if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
			throw std::runtime_error("fcntl(fd, F_SETFL, O_NONBLOCK) returned ERROR");
	}

	/* Sets socket options at socket level (SOL_SOCKET) 
	which allows the doublicate address and port bindings (SO_REUSEPORT)*/
	void Socket::SetOptions_()
	{
		int value = 1;
		if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &value, sizeof(value)) == -1)
			throw std::runtime_error("setsockopt(fd, SOL_SOCKET, ...) returned ERROR");
	}

	/* Closes internal file descriptor. Checks for potential double close */
	void Socket::Close()
	{
		if (fd >= 0)
			close(fd);
		fd = -1;
	}

	/* Returns the internal file descriptor */
	int Socket::GetFd() const { return fd; }

	/* Accepts an incoming request for a new client Connection */
	ClientSocket* ServerSocket::Accept()
	{
		struct sockaddr_in client_address;
		socklen_t size = sizeof(client_address);

		int new_socket = accept(fd, (sockaddr *) &client_address, &size);
		if (new_socket == -1)
			throw std::runtime_error("accept(fd, ...) returned ERROR");
		return new ClientSocket(new_socket);
	}
}
