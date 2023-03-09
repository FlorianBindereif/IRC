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
			Socket			socket_;
			bool 			status_;
		public:
							Connection(int fd);
			virtual			~Connection(){};
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
	};
	
	class ServerConnection: public Connection
	{
		public:
			ServerConnection();
		public:
			void 				Bind(int port);
			void 				Listen();
			void 				SetOptions();
			int					Accept();
		public:
			void Receive(){};
			void Send(){};
	};
}

#endif // CONNECTION_HPP