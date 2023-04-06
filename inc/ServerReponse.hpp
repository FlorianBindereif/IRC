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
	std::string ERR_CANNOTSENDTOCHAN(const std::string& nick, const std::string& channel_name);
	std::string ERR_NOTREGISTERED(std::string& command);
	std::string ERR_NOSUCHCHANNEL(std::string& nick, std::string& channel_name);
	std::string ERR_USERSDONTMATCH(std::string& nick, std::string& target);
	std::string RPL_MODEUSER(std::string& nick, const std::string& mode);
	std::string ERR_UMODEUNKNOWNFLAG(std::string& nick);
	std::string ERR_CHANOPRIVSNEEDED(std::string& nick, const std::string& channel_name);
	std::string RPL_CHANNELMODEIS(std::string& nick, std::string& channel_name, const std::string& mode);
	std::string RPL_SETMODECHANNEL(const std::string& nick, const std::string& channel_name, const std::string& mode);
	std::string ERR_NOSUCHNICK(const std::string& nick, const std::string channel_name = "server");
	std::string RPL_NAMREPLY(const std::string& nick, const std::string& channel_name, const std::string& names_list);
	std::string RPL_ENDOFNAMES(const std::string& nick, const std::string& channel_name);
	std::string RPL_CAP();
	std::string	RPL_PRIVMSG(const std::string& nick, const std::string& user, const std::string& target, const std::string& msg);
	std::string RPL_WELCOME(std::string& nick, std::string user);
	std::string RPL_NICKCHANGE(std::string& old_nick, std::string& new_nick, std::string& user);
	std::string RPL_JOIN(std::string& nick, std::string& user, std::string& channel_name);
	std::string RPL_PING(std::string& nick, std::string& token);
	std::string RPL_SETMODECLIENT(const std::string& nick, const std::string& user, const std::string& channel_name, const std::string& mode, const std::string& target);
	std::string ERR_NOTONCHANNEL(const std::string nick, const std::string channel_name);
	std::string RPL_PART(const std::string& nick, const std::string& user, const std::string& channel_name, const std::string reason = " goodbye");
	std::string ERR_USERONCHANNEL(std::string& nick, const std::string& channel_name, const std::string& target);
	std::string	RPL_INVITING(const std::string& nick, const std::string& channel_name, const std::string& target);
	std::string	RPL_INVITED(const std::string& nick, const std::string& user, const std::string& channel_name, const std::string& target);
	std::string ERR_INVITEONLYCHAN(std::string& nick, const std::string& channel_name);
	std::string	RPL_NOTICE(const std::string& nick, const std::string& user, const std::string& target, const std::string& msg);
	std::string RPL_NOTOPIC(std::string& nick, const std::string& channel_name);
	std::string RPL_TOPIC(std::string& nick, const std::string& channel_name, const std::string& topic);
	std::string RPL_TOPICCHANGE(const std::string& nick, const std::string& user, const std::string& channel_name, const std::string& topic);
}

#endif