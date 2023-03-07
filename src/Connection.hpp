#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Socket.hpp"
#include <string>
#include <iostream>

namespace irc
{
	class Connection
	{
		protected:
			Socket socket_;
		public:
			Connection(int fd);
			virtual			~Connection(){};
			int				GetFd() const;
			virtual void	Receive() = 0;
			virtual void	Send() = 0;
			virtual void	CloseConnection() = 0;
	};

	class ClientConnection: public Connection
	{
		private:
			struct UserData
			{
				std::string	user_name;
				std::string	real_name;
				std::string	nick_name;
				std::string	password;
				bool		op;
			} user_data_;

		public:
			ClientConnection(int fd);
		public:
			void Receive(){};
			void Send(){};
			void CloseConnection(){};
	};
	
	class ServerConnection: public Connection
	{
		public:
			void Bind(int port);
			void Listen();
			void SetOptions();
		public:
			void Receive(){};
			void Send(){};
			void CloseConnection(){};
	};
}

#endif // CONNECTION_HPP