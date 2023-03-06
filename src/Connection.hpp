#include "Socket.hpp"
#include <string>

namespace irc
{
	class Connection
	{
		protected:
			Socket socket_;
		public:
			int			GetFd() const;
			virtual int Receive() = 0;
			virtual int Send() = 0;
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
			int Receive();
			int Send();
	};

	class ServerConnection: public Connection
	{
		public:
			void Bind(int port);
			void Listen();
			int Receive();
			int Send();
	};
}