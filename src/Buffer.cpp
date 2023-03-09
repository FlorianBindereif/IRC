#include "../inc/Buffer.hpp"

namespace irc
{
	Buffer::Buffer()
	{ buffer_.reserve(1024); }

	void Buffer::Append(char *content)
	{ buffer_.append(content); }

	bool Buffer::Empty() const
	{ return buffer_.empty(); }

	void Buffer::Clear()
	{ return buffer_.clear(); }

	const char* Buffer::C_str() const 
	{ return buffer_.c_str(); }

	const std::string& Buffer::GetBuffer() const
	{ return buffer_; }

	std::ostream& operator<<(std::ostream& os, const Buffer& obj)
	{
		os << obj.GetBuffer();
		return os;
	}
}
