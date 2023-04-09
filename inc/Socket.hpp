#ifndef SOCKET_HPP
#define SOCKET_HPP

namespace irc
{
	class Socket
	{
	protected:
		int fd_;

	private:
		Socket(const Socket &other);

	public:
		Socket();
		Socket(int fd);
		virtual ~Socket();

	private:
		void SetNonBlocking_();

	public:
		int GetFd() const;
		void Close();
	};
}

#endif