#include "Socket.hpp"
#include <string>

namespace irc
{
	namespace connection
	{
		class Connection
		{
			public:
				virtual int Receive() = 0;
				virtual int Send() = 0;
		};

		class ClientConnection: public Connection
		{
			private:
				irc::net::ClientSocket socket_;

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
				int Receive();
				int Send();
		};
	}
}