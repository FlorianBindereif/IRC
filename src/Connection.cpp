#include "../inc/Connection.hpp"

namespace irc
{
	Connection::Connection() : socket_(), state(CONNECTED) {}

	Connection::Connection(int fd) : socket_(fd), state(CONNECTED) {}

	Connection::~Connection() {}

	int Connection::GetFd() const { return socket_.GetFd(); }

	ConnectionState &Connection::GetStatus() { return state; }
}