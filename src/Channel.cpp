#include "../inc/Channel.hpp"

namespace irc
{
	Channel::Channel(std::string name) : name_(name) {}

	void Channel::AddConnection(ClientConnection *to_join, unsigned char permission = MEMBER)
	{
		registered_.insert(std::make_pair(to_join, permission));
	}

	void Channel::RemoveConnection(ClientConnection *to_leave)
	{
		std::map<ClientConnection *, unsigned char>::iterator it = registered_.find(to_leave);
		if (it != registered_.end())
			registered_.erase(to_leave);
	}

	void Channel::Broadcast(const std::string &message, std::string exlude_nick)
	{
		for (std::map<ClientConnection *, unsigned char>::iterator it = registered_.begin(); it != registered_.end(); it++)
		{
			if (it->first->GetStatus() != DISCONNECTED)
			{
				if (!exlude_nick.empty() && it->first->user.nick == exlude_nick)
					continue;
				it->first->output_buffer.Append(message);
			}
		}
	}

	bool Channel::IsOperator(ClientConnection *connection)
	{
		std::map<ClientConnection *, unsigned char>::iterator it = registered_.find(connection);
		return it == registered_.end() ? false : (it->second & OPERATOR) == OPERATOR;
	}

	bool Channel::IsInvis(ClientConnection *connection)
	{
		std::map<ClientConnection *, unsigned char>::iterator it = registered_.find(connection);
		return it == registered_.end() ? false : (it->second & OPERATOR) == OPERATOR;
	}

	bool Channel::IsInvited(ClientConnection *connection)
	{
		std::vector<ClientConnection *>::iterator it = std::find(invited_.begin(), invited_.end(), connection);
		return it == invited_.end() ? false : true;
	}

	bool Channel::IsRegistered(ClientConnection *connection)
	{
		std::map<ClientConnection *, unsigned char>::iterator it = registered_.find(connection);
		return it == registered_.end() ? false : true;
	}

	void Channel::GiveOperator(ClientConnection *connection)
	{
		try
		{
			registered_.at(connection) |= OPERATOR;
		}
		catch (const std::exception &e)
		{
			std::cerr << connection->user.nick << " tried to gain op permissions but was not registered to the channel" << name_ << '\n';
		}
	}

	void Channel::TakeOperator(ClientConnection *connection)
	{
		try
		{
			registered_.at(connection) &= ~OPERATOR;
		}
		catch (const std::exception &e)
		{
			std::cerr << connection->user.nick << " tried to lose op permissions but was not registered to the channel" << name_ << '\n';
		}
	}

	void Channel::GiveInvite(ClientConnection *connection)
	{
		std::vector<ClientConnection *>::iterator it = std::find(invited_.begin(), invited_.end(), connection);
		if (it == invited_.end())
			invited_.push_back(connection);
	}

	void Channel::TakeInvite(ClientConnection *connection)
	{
		std::vector<ClientConnection *>::iterator it = std::find(invited_.begin(), invited_.end(), connection);
		if (it != invited_.end())
			invited_.erase(it);
	}

	void Channel::GiveInvis(ClientConnection *connection)
	{
		try
		{
			registered_.at(connection) |= INVIS;
		}
		catch (const std::exception &e)
		{
			std::cerr << connection->user.nick << " tried to gain invisibility but was not registered to the channel" << name_ << '\n';
		}
	}

	void Channel::TakeInvis(ClientConnection *connection)
	{
		try
		{
			registered_.at(connection) &= ~INVIS;
		}
		catch (const std::exception &e)
		{
			std::cerr << connection->user.nick << " tried to lose invisibility but was not registered to the channel" << name_ << '\n';
		}
	}

	void Channel::SetChannelMode(std::string mode)
	{
		bool add = true;

		for (std::string::size_type i = 0; i < mode.size(); i++)
		{
			if (mode[i] == '-')
				add = false;
			else if (mode[i] == '+')
				add = true;
			else if (mode[i] == 'm' && add)
				channel_mode_ |= CHANMOD;
			else if (mode[i] == 'm' && !add)
				channel_mode_ &= ~CHANMOD;
			else if (mode[i] == 'i' && add)
				channel_mode_ |= INVIS;
			else if (mode[i] == 'i' && !add)
				channel_mode_ &= ~INVIS;
			else if (mode[i] == 't' && add)
				channel_mode_ |= CHANTOPIC;
			else if (mode[i] == 't' && !add)
				channel_mode_ &= ~CHANTOPIC;
			else if (mode[i] == 'b' && add)
				channel_mode_ |= CHANBOT;
			else if (mode[i] == 'b' && !add)
				channel_mode_ &= ~CHANBOT;
		}
	}

	void Channel::SetRegisteredMode(ClientConnection *target, std::string &mode)
	{
		bool add;
		for (std::string::size_type i = 0; i < mode.size(); i++)
		{
			if (mode[i] == '-')
				add = false;
			else if (mode[i] == '+')
				add = true;
			else if (mode[i] == 'o' && add)
				GiveOperator(target);
			else if (mode[i] == 'o' && !add)
				TakeOperator(target);
			else if (mode[i] == 'i' && add)
				GiveInvis(target);
			else if (mode[i] == 'i' && !add)
				TakeInvis(target);
		}
	}

	std::string Channel::GetRegisteredString(bool include_invis)
	{
		std::string name_string;
		for (std::map<ClientConnection *, unsigned char>::iterator it = registered_.begin(); it != registered_.end(); it++)
		{
			if (include_invis == false && ((*it).first->client_mode & INVIS))
				continue;
			if (((*it).second & OPERATOR))
				name_string += "@";
			name_string += (*it).first->user.nick + " ";
		}
		return name_string;
	}

	unsigned char Channel::GetMode() const { return channel_mode_; }

	std::string Channel::GetModeString() const
	{
		std::string mode("+");
		if (channel_mode_ & CHANMOD)
			mode += "m";
		if (channel_mode_ & CHANINVITE)
			mode += "i";
		if (channel_mode_ & CHANTOPIC)
			mode += "t";
		if (channel_mode_ & CHANBOT)
			mode += "b";
		return mode;
	}

	std::string Channel::GetName() const { return name_; }

	void Channel::SetTopic(std::string &topic) { topic_ = topic; }

	std::string Channel::GetTopic() { return topic_; }

	bool Channel::IsEmpty() { return registered_.empty(); }
}