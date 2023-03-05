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
	namespace net
	{
		/* Constructs a new stream socket base element*/
		Socket::Socket()
		{
			fd = socket(AF_INET, SOCK_STREAM, 0);
			if (fd == -1)
				throw std::runtime_error("socket(PF_INET, SOCKSTREAM, 0) returned ERROR");
			SetOptions_();
			SetNonBlocking_();
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

		/* Constructs a ServerSocket */
		ServerSocket::ServerSocket(){ }

		/* Destructs the ServerSocket*/
		ServerSocket::~ServerSocket() { }

		/* Binds ServerSocket to @port */
		void ServerSocket::Bind(int port)
		{
			struct sockaddr_in server_address;

			memset(&server_address, 0, sizeof(server_address));
			server_address.sin_family = AF_INET;
			server_address.sin_addr.s_addr = inet_addr(IP);
			server_address.sin_port = htons(port);

			if (bind(fd, (struct sockaddr *) &server_address, sizeof(server_address)) == -1)
				throw std::runtime_error("bind(fd, ...) returned ERROR");
		}

		/* Sets ServerSocket into listening mode for @BACKLOG elements in queue */
		void ServerSocket::Listen() { listen(fd, BACKLOG); }

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

	} // irc net
}
