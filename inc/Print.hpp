#ifndef COLOUR_HPP
#define COLOUR_HPP

#define RESET	"\033[0m"
#define RED		"\033[31m"
#define GREEN	"\033[32m"
#define BLUE    "\033[34m"
#define WHITE	"\033[37m"
#define YELLOW	"\x1B[43m"

#define PRINTNL(message, colour) \
	(std::cout << colour << message << RESET << "\n")
#endif