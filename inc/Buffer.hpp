#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "config.hpp"
#include <string>
#include <iostream>

namespace irc
{
	class Buffer
	{
		private:
			std::string buffer_;
		public:
			Buffer();
		public:
			void				Append(char *content, size_t subpos, size_t sublen);
			void 				Append(std::string content);
			bool				Empty() const;
			void				Clear();
			const char* 		C_str() const;
			bool 				HoldsCommand() const;
			std::string 		GetCommand();
			std::string 		GetCommandCR();
			const std::string& 	GetBuffer() const;
	};
	
	std::ostream& operator<<(std::ostream& os, const Buffer& obj);
}

#endif