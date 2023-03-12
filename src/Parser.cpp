#include "../inc/Parser.hpp"

namespace irc
{

	Message MessageParser::Parse(std::string &input)
	{
		Message message;

		input_ = input;
		ParsePrefix_(message.prefix);
		ParseCommand_(message.command);\
		ParseParams_(message.params);
	}

	void MessageParser::ParsePrefix_(Prefix& prefix)
	{
		if (input_[0] == ':')
		{
			size_type end = input_.find(' ');
			if (end == not_found)
				throw std::invalid_argument("Invalid Prefix: Not followed by a command!");
			std::string prefix_string = input_.substr(1, end - 1);
			input_.erase(0, end + 1);
			std::size_t user = prefix_string.find('!');
			std::size_t host = prefix_string.find('@');
			if (user != not_found && host != not_found)
			{
				if (user < host)
				{
					prefix.nickname = prefix_string.substr(0, user);
					prefix.user = prefix_string.substr(user + 1, host - user - 1);
					prefix.host = prefix_string.substr(host + 1);
				}
				else
					throw std::invalid_argument("Invalid Prefix: host listed before user");
			}
			else if (host != not_found)
			{
				prefix.nickname = prefix_string.substr(0, host);
				prefix.host = prefix_string.substr(host + 1);
			}
			else
				prefix.nickname = prefix_string;
		}
	}

	void MessageParser::ParseCommand_(std::string& command)
	{
		size_type end = input_.find(' ');
		if (end != not_found)
		{
			command = input_.substr(1, end - 1);
			input_.erase(0, end + 1);
		}
		else
			command = input_;
	}

	void ParseParams_(std::vector<std::string>& params)
	{
	}
}