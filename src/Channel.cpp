#include "../inc/Channel.hpp"

namespace irc
{
	Channel::Channel(std::string name): name_(name) { }

	void Channel::AddConnection(ClientConnection* to_join)
	{ connections_.insert(to_join); }

	void Channel::RemoveConnection(ClientConnection* to_leave)
	{ connections_.erase(to_leave); }

	void Channel::Broadcast(std::string& message) const
	{ }
}