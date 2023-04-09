#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <iostream>
#include <vector>

namespace irc
{
	struct Prefix
	{
		std::string name;
		std::string user;
		std::string host;
	};

	class Message
	{
	public:
		Prefix prefix;
		std::string command;
		std::vector<std::string> middle_params;
		std::string trailing;

		friend std::ostream &operator<<(std::ostream &os, const Message &message);
	};

	class MessageParser
	{
	private:
		typedef std::string::size_type size_type;

	private:
		static std::string to_parse_;

	public:
		MessageParser();

	private:
		static void ParsePrefix_(Prefix &prefix);
		static void ParseCommand_(std::string &command);
		static void ParseParams_(std::vector<std::string> &params, std::string &trailing);

	public:
		static Message Parse(std::string &to_parse);
	};
}

#endif