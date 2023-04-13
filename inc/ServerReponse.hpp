#ifndef SERVERRESPONSES_HPP
#define SERVERRESPONSES_HPP

#include <string>

namespace irc
{
	/***********************************************
		ERROR REPLIES
	***********************************************/
	std::string ERR_NEEDMOREPARAMS(const std::string &command);
	std::string ERR_PASSWDMISMATCH();
	std::string ERR_ALREADYREGISTRED();
	std::string ERR_NICKNAMEINUSE(const std::string &nick);
	std::string ERR_NONICKNAMEGIVEN();
	std::string ERR_NOTREGISTERED(const std::string &command);
	std::string ERR_NOSUCHCHANNEL(const std::string &nick, const std::string &channel_name);
	std::string ERR_ERRONEUSNICKNAME(const std::string &nick, const std::string &wrong_nick);
	std::string ERR_USERSDONTMATCH(const std::string &nick, const std::string &target);
	std::string ERR_UNKNOWNCOMMAND(const std::string &nick, const std::string &command);
	std::string ERR_NOSUCHNICK(const std::string &nick, const std::string channel_name = "server");
	std::string ERR_CANNOTSENDTOCHAN(const std::string &nick, const std::string &channel_name);
	std::string ERR_CHANOPRIVSNEEDED(const std::string &nick, const std::string &channel_name);
	std::string ERR_NOPRIVS(const std::string &nick, const std::string &command);
	std::string ERR_UMODEUNKNOWNFLAG(const std::string &nick);
	std::string ERR_USERONCHANNEL(const std::string &nick, const std::string &channel_name, const std::string &target);
	std::string ERR_USERNOTINCHANNEL(const std::string &nick, const std::string &channel_name);
	std::string ERR_NOTONCHANNEL(const std::string nick, const std::string channel_name);
	std::string ERR_INVITEONLYCHAN(const std::string &nick, const std::string &channel_name);

	/***********************************************
		COMMAND REPLIES
	***********************************************/
	std::string RPL_NICKCHANGE(const std::string &old_nick, const std::string &new_nick, const std::string &user);
	std::string RPL_CAP();
	std::string RPL_JOIN(const std::string &nick, const std::string &user, const std::string &channel_name);
	std::string RPL_PING(const std::string &nick, const std::string &token);
	std::string RPL_SETMODECLIENT(const std::string &nick, const std::string &user, const std::string &channel_name, const std::string &mode, const std::string &target);
	std::string RPL_PART(const std::string &nick, const std::string &user, const std::string &channel_name, const std::string reason = " goodbye");
	std::string RPL_PRIVMSG(const std::string &nick, const std::string &user, const std::string &target, const std::string &msg);
	std::string RPL_NOTICE(const std::string &nick, const std::string &user, const std::string &target, const std::string &msg);
	std::string RPL_TOPICCHANGE(const std::string &nick, const std::string &user, const std::string &channel_name, const std::string &topic);
	std::string RPL_KICK(const std::string &nick, const std::string &user, const std::string &channel_name, const std::string &target, std::string reason = "no reason stated");
	std::string RPL_QUIT(const std::string &nick, const std::string &user);
	std::string RPL_BOT(const std::string &nick, const std::string &channel_name, const std::string &msg);
	std::string RPL_INVITED(const std::string &nick, const std::string &user, const std::string &channel_name, const std::string &target);

	/***********************************************
		NUMERIC REPLIES
	***********************************************/
	std::string RPL_WELCOME(const std::string &nick, const std::string user);
	std::string RPL_NAMREPLY(const std::string &nick, const std::string &channel_name, const std::string &names_list);
	std::string RPL_ENDOFNAMES(const std::string &nick, const std::string &channel_name);
	std::string RPL_MODEUSER(const std::string &nick, const std::string &mode);
	std::string RPL_CHANNELMODEIS(const std::string &nick, const std::string &channel_name, const std::string &mode);
	std::string RPL_SETMODECHANNEL(const std::string &nick, const std::string &channel_name, const std::string &mode);
	std::string RPL_YOUREOPER(const std::string &nick, const std::string &target);
	std::string RPL_INVITING(const std::string &nick, const std::string &channel_name, const std::string &target);
	std::string RPL_NOTOPIC(const std::string &nick, const std::string &channel_name);
	std::string RPL_TOPIC(const std::string &nick, const std::string &channel_name, const std::string &topic);
}

#endif