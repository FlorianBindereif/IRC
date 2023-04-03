#include "../inc/Channel.hpp"

namespace irc
{
	Channel::Channel(std::string name): name_(name) { }

	void Channel::AddConnection(ClientConnection* to_join, Permission permission = USER)
	{ registered_.insert(std::make_pair(to_join, permission)); }

	void Channel::RemoveConnection(ClientConnection* to_leave)
	{ registered_.erase(to_leave); }

	void Channel::Broadcast(const std::string& message)
	{
		for (std::map<ClientConnection *, Permission>::iterator it = registered_.begin(); it != registered_.end(); it++)
		{
			if (it->first->GetStatus() != DISCONNECTED)
				it->first->output_buffer_.Append(message);
		}
	}

	Channel::Permission Channel::GetPermissions(ClientConnection* connection)
	{
		std::map<ClientConnection *, Permission>::iterator it = registered_.find(connection);
		return it == registered_.end() ? NOTREGISTERED : it->second;
	}

	void Channel::GiveOperatorPermission(ClientConnection* connection)
	{
		try
		{ registered_.at(connection) = OPERATOR; }
		catch(const std::exception& e)
		{ std::cerr << connection->user.nick << " tried to gain op permissions but was not registered to the channel" << name_ << '\n'; }
	}		

	void Channel::TakeOperatorPermission(ClientConnection* connection)
	{ 	try
		{ registered_.at(connection) = USER; }
		catch(const std::exception& e)
		{ std::cerr << connection->user.nick << " tried to lose op permissions but was not registered to the channel" << name_ << '\n'; } 
	}

	void Channel::SetChannelMode(std::string mode)
	{
		bool	add = true;

		for (std::string::size_type i = 0; i < mode.size(); i++)
		{
			if (mode[i] == '-')
				add = false;
			else if (mode[i] == '+')
				add = true;
			else if (mode[i] == 'm' && add)
				mode_ |= 0b001;
			else if (mode[i] == 'm' && !add)
				mode_ &= 0b110;
			else if (mode[i] == 'i' && add)
				mode_ |= 0b010;
			else if (mode[i] == 'i' && !add)
				mode_ &= 0b101;
			else if (mode[i] == 't' && add)
				mode_ |= 0b100;
			else if (mode[i] == 't' && !add)
				mode_ &= 0b011;
			else if (mode[i] == 'b' && add)
				mode_ |= 0b1000;
			else if (mode[i] == 'b' && !add)
				mode_ &= 0b0111;
		}
	}

	void Channel::SetRegisteredMode(ClientConnection* target, std::string& mode)
	{
		if (mode.find_first_of("+o") != std::string::npos)
			GiveOperatorPermission(target);
		else if (mode.find_first_of("-o") != std::string::npos)
			TakeOperatorPermission(target);
	}

	std::string Channel::GetRegisteredString()
	{
		std::string name_string;
		for (std::map<ClientConnection *, Permission>::iterator it = registered_.begin(); it != registered_.end(); it++)
		{
			if ((*it).second == OPERATOR)
				name_string += "@";
			name_string += (*it).first->user.nick + " ";
		}
		return name_string;
	}

	unsigned char Channel::GetMode() const
	{ return mode_; }

	std::string Channel::GetModeString() const
	{
		std::string	mode("+");
		if (mode_ & 0b0001)
			mode += "m";
		if (mode_ & 0b0010)
			mode += "i";
		if (mode_ & 0b0100)
			mode += "t";
		if (mode_ & 0b1000)
			mode += "b";
		return mode;
	}

	std::string Channel::GetName() const
	{ return name_; }
}