#include "../inc/Parser.hpp"
#include <sstream>

namespace irc
{
	Message MessageParser::Parse(std::string &input)
	{
		Message message;

		input_ = input;
		ParsePrefix_(message.prefix);
		ParseCommand_(message.command);
		ParseParams_(message.middle_params, message.trailing);
		return message;
	}

	void MessageParser::ParsePrefix_(Prefix& prefix)
	{
		if (input_.front() == ':')
		{
			size_type end = input_.find(' ');
			if (end == std::string::npos)
				throw std::invalid_argument("Invalid Prefix: Not followed by a command!");
			std::string prefix_string = input_.substr(1, end - 1);
			input_.erase(0, end + 1);
			std::size_t user = prefix_string.find('!');
			std::size_t host = prefix_string.find('@');
			if (user != std::string::npos && host != std::string::npos)
			{
				if (user < host)
				{
					prefix.name = prefix_string.substr(0, user);
					prefix.user = prefix_string.substr(user + 1, host - user - 1);
					prefix.host = prefix_string.substr(host + 1);
				}
				else
					throw std::invalid_argument("Invalid Prefix: host listed before user");
			}
			else if (host != std::string::npos)
			{
				prefix.name = prefix_string.substr(0, host);
				prefix.host = prefix_string.substr(host + 1);
			}
			else
				prefix.name = prefix_string;
		}
	}

	void MessageParser::ParseCommand_(std::string& command)
	{
		size_type end = input_.find(' ');
		if (end != std::string::npos)
		{
			command = input_.substr(1, end - 1);
			input_.erase(0, end + 1);
		}
		else
			command = input_;
	}

	void MessageParser::ParseParams_(std::vector<std::string>& params, std::string& trailing)
	{
		std::istringstream iss(input_);
		std::string token;

		while(iss >> token)
		{
			if (token.front() == ':')
			{
				trailing.append(token.substr(1));
				while(iss >> token)
					trailing.append(" " + token);
				break;
			}
			params.push_back(token);
		}
	}

	std::ostream& operator<<(std::ostream& os, const Message& message)
	{
		os << "Prefix:" << "\n";
		os << "nick:\t\t" << message.prefix.name << "\n";
		os << "user:\t\t" << message.prefix.user << "\n";
		os << "host:\t\t" << message.prefix.host << "\n";
		os << "\n";
		os << "Command:\t" << message.command << "\n";
		os << "Parameters:\t" << message.command << "\n";
		for (std::vector<std::string>::const_iterator it = message.middle_params.begin(); it != message.middle_params.end(); ++it)
			os << "| " << *it;
		os << "\n";
		os << "Trailing:\t" << message.trailing << "\n";
		os << std::endl;
		return os;
	}
}