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
			void Append(char *content);
			bool Empty() const;
			void Clear();
			const char* C_str() const;
			const std::string& GetBuffer() const;
	};
	
	std::ostream& operator<<(std::ostream& os, const Buffer& obj);
}

#endif