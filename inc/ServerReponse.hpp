#ifndef SERVER_RESPONSES
#define SERVER_RESPONSES

#include <string>

namespace irc
{
	//PASSWORD
	std::string ERR_PASSWDMISMATCH();
	std::string ERR_NEEDMOREPARAMS(std::string& command);
	std::string ERR_ALREADYREGISTRED();

	//NICKNAME
	std::string ERR_NICKNAMEINUSE(std::string& nick);
	std::string ERR_NONICKNAMEGIVEN();
	std::string ERR_NOTREGISTERED(std::string& command);
	std::string ERR_NOSUCHCHANNEL(std::string& nick, std::string& channel_name);
	std::string ERR_BADCHANNELFORMAT(std::string& nick, std::string& channel_name);


	std::string RPL_CAP();
	std::string RPL_WELCOME(std::string& nick, std::string user);
	std::string RPL_NICKCHANGE(std::string& old_nick, std::string& new_nick, std::string& user);
	std::string RPL_JOIN(std::string& nick, std::string& user, std::string& channel_name);
	std::string RPL_PING(std::string& nick, std::string& token);

}
#endif