#include "../inc/Channel.hpp"

namespace irc
{
	Channel::Channel(std::string name): name_(name) { }

	void Channel::AddConnection(ClientConnection* to_join)
	{ registered_.insert(std::make_pair(to_join, USER)); }

	void Channel::RemoveConnection(ClientConnection* to_leave)
	{ registered_.erase(to_leave); }

	void Channel::Broadcast(std::string& message) const
	{ (void) message; }

	void Channel::GiveOperatorPermission(ClientConnection* connection)
	{
		try
		{ registered_.at(connection) = OPERATOR; }
		catch(const std::exception& e)
		{ std::cerr << e.what() << '\n'; }
	}		

	void Channel::TakeOperatorPermission(ClientConnection* connection)
	{ 	try
		{ registered_.at(connection) = USER; }
		catch(const std::exception& e)
		{ std::cerr << e.what() << '\n'; } 
	}
}