#ifndef CONFIG_HPP
#define CONFIG_HPP

namespace irc
{
	//Names the Server for response-generation
	#define SERVERNAME "idiotic.chat"
	
	//Names the Host for response-generation
	#define HOST "localhost"

	//Number of pending connections the serverque will hold 
	#define BACKLOG	128

	//Length of accepted IRC message including CR-LF
	#define MESSAGE_LENGTH 512

	//IP the Server will be listening to
	#define IP "0.0.0.0"

	//Password required Client to connect to the Server
	#define PASSWORD "pwd"

	//Default Port the Server will bind to
	#define PORT 6060

	//Pollevents to listen to
	#define POLLEVENTS (POLLIN|POLLOUT|POLLERR|POLLHUP|POLLNVAL)
}
#endif