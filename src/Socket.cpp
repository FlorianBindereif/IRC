#include "../inc/Socket.hpp"
#include <sys/socket.h>
#include <fcntl.h>
#include <stdexcept>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include "../inc/config.hpp"
#include <iostream>

namespace irc
{
	/* Constructs a new stream socket base element*/
	Socket::Socket()
	{
		try
		{
			fd_ = socket(AF_INET, SOCK_STREAM, 0);
			if (fd_ == -1)
				throw std::runtime_error("socket(PF_INET, SOCKSTREAM, 0) returned ERROR");
			SetNonBlocking_();
		}
		catch(const std::exception& error)
		{ 
			std::cerr << error.what() << '\n';
			Close();
			throw ;
		}
	}

	Socket::Socket(int fd):fd_(fd)
	{
		try
		{
			SetNonBlocking_();
		}
		catch(const std::exception& error)
		{ 
			std::cerr << error.what() << '\n';
			Close();
			throw ;
		}
	}

	/* Destructs the Socket object and closes the internal file descriptor*/
	Socket::~Socket()
	{ Close(); }
	
	/*Sets socket to non-blocking state*/
	void Socket::SetNonBlocking_()
	{
		if (fcntl(fd_, F_SETFL, O_NONBLOCK) == -1)
			throw std::runtime_error("fcntl(fd_, F_SETFL, O_NONBLOCK) returned ERROR");
	}

	/* Closes internal file descriptor. Checks for potential double close */
	void Socket::Close()
	{
		if (fd_ >= 0)
			close(fd_);
		fd_ = -1;
	}

	/* Returns the internal file descriptor */
	int Socket::GetFd() const { return fd_; }
}