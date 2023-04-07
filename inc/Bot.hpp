#ifndef BOT_HPP
#define BOT_HPP

#include <string>
#include "Parser.hpp"
#include "Connection.hpp"

namespace irc
{
	std::string BlackJackBot(ClientConnection* client, Message& message);
}

#endif