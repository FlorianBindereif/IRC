namespace IRC
{
	//Number of pending connections the serverque will hold 
	#define BACKLOG	128

	//Length of accepted IRC message including CR-LF
	#define MESSAGE_LENGTH 512

	//IP the Server will be listening to
	#define IP	"129.168.0.0"

	//Password required Client to connect to the Server
	#define PASSWORD "IRC42"

	//Default Port the Server will bind to
	#define PORT 6660

	//Pollevents to listen to
	#define POLLEVENTS (POLLIN|POLLOUT|POLLERR|POLLHUP|POLLNVAL)

		
}