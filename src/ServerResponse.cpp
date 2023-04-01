#include "../inc/ServerReponse.hpp"
#include "../inc/config.hpp"
#include "../inc/Print.hpp"
#include <iostream>

namespace irc
{
	std::string ERR_PASSWDMISMATCH()
	{ 
		std::cout << RED << "User provided invalid password!" << RESET << "\n";
		return std::string(":") + SERVERNAME + " 464 " + "PASS" + " :Password incorrect\r\n";
	}

	std::string ERR_NEEDMOREPARAMS(std::string& command) 
	{ 
		std::cout << RED << command << ": more Parameters needed to execute command: " << command << "!" << RESET << "\n";
		return std::string(":") + SERVERNAME + " 461 " + command + " :Not enough parameters\r\n";
	}
	std::string ERR_ALREADYREGISTRED() 
	{ 
		std::cout << RED << "User tried to re-authenticate!" << RESET << "\n";
		return std::string(":") + SERVERNAME + " 462 " ":Already registered in\r\n";
	}
	std::string ERR_NICKNAMEINUSE(std::string& nick) 
	{ 
		std::cout << RED << "User tried claiming nickname already in use!" << RESET << "\n";
		return std::string(":") + SERVERNAME " 433 " + nick + " :Nickname is already in use\r\n"; 
	}

	std::string ERR_NONICKNAMEGIVEN() 
	{ 
		std::cout << RED << "User did not provide nickname!" << RESET << "\n";
		return std::string(":") + SERVERNAME + " 431 " + "NICK" + " :Nickname not given\r\n"; 
	}

	std::string RPL_NICKCHANGE(std::string& old_nick, std::string& new_nick, std::string& user) 
	{ 
		std::cout << GREEN << "User changed his nickname from " << old_nick << " to " << new_nick << "!" << RESET << "\n";
		return std::string(":") + old_nick + "!" + user + "@" + HOST + " " + "NICK" + " :" + new_nick + "\r\n";
	}

	std::string RPL_CAP()
	{
		std::cout << GREEN << "Server-capabilities were sent to client!" << RESET << "\n";
		return std::string(":") + SERVERNAME + " CAP * LS :cap reply...\r\n"; 
	}

	std::string RPL_WELCOME(std::string& nick, std::string user) 
	{
		std::cout << GREEN << "User: " << user << " succesfully registered to the server, using nick " << nick << "!" << RESET << "\n";
		return std::string(":") + SERVERNAME + " 001 " + nick + " :Welcome to the ft_irc network " + nick + "!" + user + "@" + HOST + "\r\n";
	}

	std::string ERR_NOTREGISTERED(std::string& command)
	{		
		std::cout << RED << "User tried executing command " << command << " but was not registred!" << RESET << "\n";
		return std::string(":") + SERVERNAME + " 451 " + ":You have not registered\r\n";
	}

	std::string ERR_NOSUCHCHANNEL(std::string& nick, std::string& channel_name)
	{
		std::cout << RED << "User tried accessing channel " << channel_name << " but it did not exist" << RESET << "\n";
		return std::string(":") + SERVERNAME + " 403 " +  nick + " " + channel_name + " : No such channel\r\n";
	}

	std::string RPL_JOIN(std::string& nick, std::string& user, std::string& channel_name)
	{
		std::cout << GREEN << nick << " joined channel " << channel_name << "!" << RESET << "\n";
		return std::string(":") + nick + "!" + user + "@" + HOST + " JOIN " + channel_name + " * :" + user + "\r\n";
	}

	std::string RPL_PING(std::string& nick, std::string& token)
	{
		std::cout << GREEN << nick << " pinged this server! " << RESET << "\n";
		return std::string(":") + SERVERNAME + " PONG " + SERVERNAME + " :" + token + "\r\n";
	}

	// std::string RPL_NAMREPLY(std::string& nick, std::string& channel_name)
	// {
	// 	//
	// }	return std::string(":") + SERVERNAME + " 353 " + nick + " = " + channel + " :"

	std::string ERR_USERSDONTMATCH(std::string& nick, std::string& target)
	{
		std::cout << RED << nick << "tried changing mode for " << target << "!" << RESET << "\n";
		return std::string(":") + SERVERNAME + " 502 " + nick + " : Cannot change mode for other users\r\n";
	}

	std::string RPL_MODEUSER(std::string& nick, const std::string& mode)
	{
		std::cout << GREEN << nick << " set his mode to " << mode << RESET << "\n";
		return std::string(":") + SERVERNAME + " 221 " + nick + " " + mode + "\r\n";
	}

	std::string ERR_UMODEUNKNOWNFLAG(std::string& nick)
	{
		std::cout << RED << nick << "used invalid mode flags" << RESET << "\n";
		return std::string(":") + SERVERNAME + " 501 " + nick + " :Unknown MODE flags\r\n";
	}

	std::string RPL_CHANNELMODEIS(std::string& nick, std::string& channel_name, const std::string& mode)
	{
		std::cout << GREEN << nick << "requested " << channel_name << "'s mode: " << mode << RESET << "\n";
		return std::string(":") + SERVERNAME + " 324 " + nick + " " + channel_name + " " + mode + "\r\n";
	}

	std::string RPL_SETMODECHANNEL(const std::string& nick, const std::string& channel_name, const std::string& mode)
	{
		std::cout << GREEN << nick << "set " << channel_name << "'s mode to " << mode << "!" << RESET << "\n";
		return std::string(":") + SERVERNAME + " 324 " + nick + " " + channel_name + " " + mode + "\r\n";
	}
	
	std::string ERR_NOSUCHNICK(std::string& nick)
	{
		std::cout << RED << "User tried using nick " << nick << " but it did not exist" << RESET << "\n";
		return std::string("401 ") + SERVERNAME + " " + nick + " :No such nick/channel";
	}

	// #define ERR_NOSUCHNICK(source, nickname)				"401 " + source + " " + nickname + " :No such nick/channel"

	// std::string RPL_SETMODECHANNEL(std::string& nick, std::string& user, std::string, std::string& channel_name, std::string& mode, std::string& nick)
	// {
	// 	return std::string(":") + ":" + nick + "!" + user + "@" + HOST + " MODE " + channel_name + " " + mode + " " + nick + "\r\n";
	// }

	std::string ERR_CHANOPRIVSNEEDED(std::string& nick, const std::string& channel_name)
	{
		std::cout << RED << nick << "tried changing mode for " << channel_name << " but did not have op permissions!" << RESET << "\n";
		return std::string(":") + SERVERNAME + " 482 " + nick + " " + channel_name + " :You're not channel operator \r\n";
	}			
}


// #define RPL_PING(src, token)								":" + src + " PONG " + src + " :" + token + "\r\n"
// #include <iostream>
// #include <string>
// #include <cstdlib>
// #include <cstdio>
// #include <string.h>
// #include <vector>
// /* might need to add \r\n at the end of each reply !*/
// ​
// //ERROR REPLIES
// #define ERR_ALREADYLOGEDIN(source)						":" + source + " 460 " ":Already logged in\r\n"
// ​
// #define ERR_NOTONCHANNEL(src, nick, channel)			":" + src + " 401 " + nick + " " + channel + " :No such nick/channel\r\n"
// #define ERR_CANNOTSENDTOCHAN(src, nick, channel)		":" + src +  " 404 " + nick + " " + channel + " :Cannot send to channel\r\n"
// #define	ERR_UMODEUNKNOWNFLAUSR(src, nick)				":" + src +  " 501 " + nick + " :Unknown MODE flags\r\n"
// #define	ERR_UMODEUNKNOWNFLAGCH(src, nick)				":" + src +  " 472 " + nick + " :Unknown mode char to me\r\n"
// #define ERR_CHANOPRIVSNEEDED(src, nick, channel)		":" + src +  " 482 " + nick + " " + channel + " :You're not channel operator \r\n"
// ​
// #define ERR_ERRONEUSNICKNAME(nick)						"432 " + nick + ":Erroneous nickname"
// ​
// #define ERR_UNKNOWNCOMMAND(source, command)				"421 " + source + " " + command + " :Unknown command"
// #define ERR_TOOMANYCHANNELS(source, channel)			"405 " + source + " " + channel + " :You have joined too many channels"
// #define ERR_BADCHANNELKEY(source, channel)				"475 " + source + " " + channel + " :Cannot join channel (+k)"
// #define ERR_NOSUCHNICK(source, nickname)				"401 " + source + " " + nickname + " :No such nick/channel"
// #define ERR_USERNOTINCHANNEL(source, nickname, channel)	"441 " + source + " " + nickname + " " + channel + " :They aren't on that channel"
// #define ERR_CHANNELISFULL(source, channel)				"471 " + source + " " + channel + " :Cannot join channel (+l)"
// ​
// // NUMERIC REPLIES
// #define RPL_PING(src, token)								":" + src + " PONG " + src + " :" + token + "\r\n"
// #define	RPL_TOPIC(src, nick, channel, topic)				":" + src + " 332 " + nick + " " + channel + " :" + topic + "\r\n"
// #define RPL_NOTOPIC(src, nick, channel)						":" + src + " 331 " + nick + " " + channel + " :No topic is set\r\n"
// #define	RPL_NAMREPLY(src, nick, channel)					":" + src + " 353 " + nick + " = " + channel + " :"
// #define RPL_ENDOFNAMES(src, nick, channel)					":" + src + " 366 " + nick + " " + channel + " :END of NAMES list\r\n"
// ​
// // COMMAND REPLIES
// #define RPL_PART(src_nick, src_usr, src_host, channel)				":" + src_nick + "!" + src_usr + "@" + src_host + " PART " + channel + " :gonee... :'( \r\n"
// #define RPL_PRIVMSG(src_nick, src_usr, src_host, dis_nick, msg)		":" + src_nick + "!" + src_usr + "@" + src_host + " PRIVMSG " + dis_nick + " :" + msg + "\r\n"
// #define RPL_NOTICE(src_nick, src_usr, src_host, dis_nick, msg)		":" + src_nick + "!" + src_usr + "@" + src_host + " NOTICE " + dis_nick + " :" + msg + "\r\n"
// #define RPL_QUIT(src_nick, src_usr, src_host)						":" + src_nick + "!" + src_usr + "@" + src_host + " QUIT :Client Quit \r\n"
// #define RPL_TOPICCHANGE(src_nick, src_usr, src_host, channel, topic)":" + src_nick + "!" + src_usr + "@" + src_host + " TOPIC " + channel + " :" + topic + "\r\n"
// ​
// #define RPL_KICK(source, channel, target, reason)					":" + source + " KICK " + channel + " " + target + " :" + reason
// #define RPL_MODECHANNEL(src, nick,channel, mode)					":" + src + " 324 " + nick + " " + channel + " " + mode + "\r\n"
// ​
// std::vector<std::string>	ft_split(const char buffer[1000], std::string delim);
// ​
// #endif