#include "../inc/Utils.hpp"

namespace irc
{
	bool	ValidChannelName(std::string& name)
	{
		if (name.front() != '#')
			return false;
		if (name.find_first_of("\x00\x07\x0D\x0A") != std::string::npos)
			return false;
		if (name.size() < 2 || name.size() > 50)
			return false;
		return true;
	}
}