#ifndef SERVER_RESPONSES
#define SERVER_RESPONSES

#include <string>

//PASSWORD
std::string ERR_PASSWDMISMATCH();
std::string ERR_NEEDMOREPARAMS();
std::string ERR_ALREADYREGISTRED();

//NICKNAME
std::string ERR_NICKNAMEINUSE(std::string& nick);
std::string ERR_NONICKNAMEGIVEN();

#endif