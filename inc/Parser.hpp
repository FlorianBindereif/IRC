#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <iostream>
#include <vector>

namespace irc
{	
	struct Prefix
	{
		std::string nickname;
		std::string user;
		std::string host;
	};

	class Message
	{
		public:
			Prefix						prefix;
			std::string					command;
			std::vector<std::string>	middle_params;
			std::string					trailing;

		friend	std::ostream& operator<<(std::ostream& os, const Message& message);
	};

	class MessageParser
	{
		private:
			const int not_found = std::string::npos;
			typedef std::string::size_type size_type;
		private:
			std::string input_;
		public:
			MessageParser();
		private:
			void ParsePrefix_(Prefix& prefix);
			void ParseCommand_(std::string& command);
			void ParseParams_(std::vector<std::string>& params, std::string& trailing);
		public: 
			Message Parse(std::string &input);
	};
}

#endif PARSER_HPP