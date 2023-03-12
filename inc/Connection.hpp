#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Socket.hpp"
#include "Buffer.hpp"
#include "../inc/config.hpp"
#include <string>
#include <unistd.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace irc
{
	class Connection
	{
		protected:
			Socket			socket_;
			bool 			status_;
		public:
							Connection();
							Connection(int fd);
			virtual			~Connection();
		public:
			int				GetFd() const;
			bool			GetStatus() const;
			void			CloseConnection();
		public:
			virtual void	Receive() = 0;
			virtual void	Send() = 0;
	};

	class ClientConnection: public Connection
	{
		private:
			Buffer	buffer_;

		public:
			enum State
			{
				HANDSHAKE,
				LOGIN,
				REGISTERED,
				DISCONNECTED
			};
		public:
							ClientConnection();
							ClientConnection(int fd);
			virtual 		~ClientConnection();
		public:
			void 			Receive();
			void 			Send();
		public:
			void			ExecuteCommand(std::string command);
	};

	class ServerConnection: public Connection
	{
		public:
							ServerConnection();
			virtual 		~ServerConnection();
		public:
			void 			Receive();
			void 			Send();
		public:
			void 			Bind(int port);
			void 			Listen();
			void 			SetOptions();
			int				Accept();
	};
}

#endif // CONNECTION_HPP