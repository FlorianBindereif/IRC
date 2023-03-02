#include <stdexcept>
#include <unistd.h>

namespace IRC
{
	namespace net
	{
		class Socket
		{
			protected:
				int fd;
			private:
				Socket(const Socket& other);
			private:
				void SetNonBlocking_();
				void SetOptions_();
			public:
				Socket();
				virtual ~Socket();
			public:
				int	GetFd() const;
				void Close();
		};

		class ServerSocket: public Socket
		{
			private:
				ServerSocket(const ServerSocket& other);
			public:
				~ServerSocket();
				ServerSocket();
				void listen();
				void bind(int port);
		};
	};
}
