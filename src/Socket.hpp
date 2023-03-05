namespace irc
{
	namespace net
	{
		class Socket
		{
			protected:
				int fd;
			private:
				Socket(const Socket& other);
			public:
				Socket();
				virtual ~Socket();
			private:
				void	SetNonBlocking_();
				void	SetOptions_();
			public:
				int		GetFd() const;
				void	Close();
		};

		class ClientSocket: public Socket
		{
			private:
				ClientSocket(const ClientSocket& other);
			public:
				~ClientSocket();
				ClientSocket(int socket);
		};

		class ServerSocket: public Socket
		{
			private:
				ServerSocket(const ServerSocket& other);
			public:
				~ServerSocket();
				ServerSocket();
			public:
				void			Listen();
				void 			Bind(int port);
				ClientSocket*	Accept();
		};
	};
}
