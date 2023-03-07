#ifndef SOCKET_HPP
#define SOCKET_HPP

namespace irc
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
}

#endif