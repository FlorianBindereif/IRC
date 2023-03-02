#include "Socket.hpp"
#include <sys/socket.h>
#include <fcntl.h>

namespace IRC
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

		/* Constructs a ServerSocket*/
		ServerSocket::ServerSocket(){ }

		/* Destructs the ServerSocket object*/
		ServerSocket::~ServerSocket() { }

		 
	} // IRC net
}
