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

	bool ValidNickName(std::string& name)
	{
		std::string specials("[]\\^_`{|}");

		for (std::string::size_type i = 0; i < name.size(); i++)
		{
			if (i == 0)
			{
				if (!std::isalpha(name[i]) && specials.find_first_of(name[i]) == std::string::npos)
					return false;
			}
			else if (!std::isalnum(name[i])
			&& specials.find_first_of(name[i]) == std::string::npos 
			&& name[i] != '-')
			return false;
		}
		return name.size() < 9;
	}
}