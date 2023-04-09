#ifndef SERVER_CONNECTION_HPP
#define SERVER_CONNECTION_HPP

#include "Connection.hpp"

namespace irc
{
	class ServerConnection : public Connection
	{
	public:
		ServerConnection();
		virtual ~ServerConnection();

	public:
		void Receive();
		void Send();
		void CloseConnection();

	public:
		void Bind(int port);
		void Listen();
		void SetOptions();
		int Accept();
	};
}

#endif