#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <vector>
#include <string>

#include "Socket.hpp"
#include "Buffer.hpp"

namespace irc
{
	enum ConnectionState
	{
		DISCONNECTED,
		CONNECTED,
		AUTHENTICATED,
		REGISTERED
	};

	class Connection
	{
	protected:
		Socket socket_;
		ConnectionState state;

	public:
		struct UserData
		{
			std::string nick;
			std::string username;
		} user;

	public:
		Connection();
		Connection(int fd);
		virtual ~Connection();

	public:
		int GetFd() const;
		ConnectionState &GetStatus();

	public:
		virtual void CloseConnection() = 0;
		virtual void Receive() = 0;
		virtual void Send() = 0;
	};
}

#endif // CONNECTION_HPP