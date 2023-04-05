#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Socket.hpp"
#include "Buffer.hpp"
#include <vector>

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
		public:
			unsigned char 				mode_;
			Buffer						input_buffer_;
			Buffer						output_buffer_;
			std::vector<std::string>	channel_list;
		public:
							ClientConnection();
							ClientConnection(int fd);
			virtual 		~ClientConnection();
		private:
			void 			SetChannelMode_(Message& message);
			void 			SetClientMode_(Message& message);
			std::string		GetModeString_() const;
			std::string 	CleanModeString_(std::string& mode, std::string flag_string);
			bool 			IsInChannel_(const std::string& channel_name);
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
			void 			SendPong(Message& message);
			void 			SetMode(Message& message);
			void 			SendNames(Message& message);
			void			PartChannel(Message& message);
			void 			SendMessage(Message& message);
			void 			SendNotice(Message& message);
			void			InviteClient(Message& message);
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