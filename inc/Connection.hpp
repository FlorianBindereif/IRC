#ifndef CONNECTION_HPP
#define CONNECTION_HPP

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
			Socket			socket_;
			ConnectionState	state;
		public:
			struct UserData
			{
				std::string nick;
				std::string username;
			} user;
		public:
								Connection();
								Connection(int fd);
			virtual				~Connection();
		public:	
			int					GetFd() const;
			ConnectionState&	GetStatus();
			void				CloseConnection();
		public:	
			virtual void		Receive() = 0;
			virtual void		Send() = 0;
	};

	class Message;

	class ClientConnection: public Connection
	{
		private:
			Buffer	input_buffer_;
			Buffer	output_buffer_;
		public:
							ClientConnection();
							ClientConnection(int fd);
			virtual 		~ClientConnection();
		public:
			void 			Receive();
			void 			Send();
		public:
			void			ExecuteMessage(std::string command);
			void 			SendCapabilities(Message& message);
			void			Authenticate(Message& message);
			void 			SetUsername(Message& message);
			void 			SetNickname(Message& message);
			void			JoinChannel(Message& message);
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