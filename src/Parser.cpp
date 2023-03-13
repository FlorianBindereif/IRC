#include "../inc/Parser.hpp"
#include <sstream>
#include "../inc/Print.hpp"

namespace irc

{
	std::string MessageParser::to_parse_;

	Message MessageParser::Parse(std::string &to_parse)
	{
		Message message;

		//vielleicht lieber den orginal string mitgeben
		to_parse_ = to_parse;
		MessageParser::ParsePrefix_(message.prefix);
		MessageParser::ParseCommand_(message.command);
		MessageParser::ParseParams_(message.middle_params, message.trailing);
		to_parse_.clear();
		return message;
	}

	void MessageParser::ParsePrefix_(Prefix& prefix)
	{
		if (to_parse_.front() == ':')
		{
			size_type end = to_parse_.find(' ');
			if (end == std::string::npos)
				throw std::invalid_argument("Invalid Prefix: Not followed by a command!");
			std::string prefix_string = to_parse_.substr(1, end - 1);
			to_parse_.erase(0, end + 1);
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
		size_type end = to_parse_.find(' ');
		if (end != std::string::npos)
		{
			command = to_parse_.substr(0, end);
			to_parse_.erase(0, end + 1);
		}
		else
			command = to_parse_;
	}

	void MessageParser::ParseParams_(std::vector<std::string>& params, std::string& trailing)
	{
		std::istringstream iss(to_parse_);
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
		os << "PREFIX:" << "\n";
		os << "nick:\t\t" << message.prefix.name << "\n";
		os << "user:\t\t" << message.prefix.user << "\n";
		os << "host:\t\t" << message.prefix.host << "\n";
		os << "\n";
		os << "COMMAND:\t" << message.command << "\n";
		os << "PARAMETERS:\t";
		for (size_t i = 0; i < message.middle_params.size(); i++)
		{
			os << message.middle_params[i];
			if (i + 1 < message.middle_params.size())
				os << " | ";
		}
		os << "\n";
		os << "TRAILING:\t" << message.trailing << std::endl;
		return os;
	}
}