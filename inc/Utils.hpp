#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

namespace irc
{
	bool ValidChannelName(std::string &name);
	bool ValidNickName(std::string &name);
}

#endif