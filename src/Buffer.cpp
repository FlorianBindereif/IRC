#include "../inc/Buffer.hpp"

namespace irc
{
  Buffer::Buffer() { buffer_.reserve(1024); }

  void Buffer::Append(char *content, size_t subpos, size_t sublen) { buffer_.append(content, subpos, sublen); }

  void Buffer::Append(std::string content) { buffer_.append(content); }

  bool Buffer::Empty() const { return buffer_.empty(); }

  void Buffer::Clear() { return buffer_.clear(); }

  const char *Buffer::C_str() const { return buffer_.c_str(); }

  const std::string &Buffer::GetBuffer() const { return buffer_; }

  bool Buffer::HoldsMessage() const { return buffer_.find("\r\n") == buffer_.npos ? false : true; }

  std::string Buffer::GetMessage()
  {
    int cr = buffer_.find("\r\n");
    std::string message = buffer_.substr(0, cr);
    buffer_.erase(0, cr + 2);
    return message;
  }

  std::string Buffer::GetMessageCR()
  {
    int cr = buffer_.find("\r\n");
    std::string message = buffer_.substr(0, cr + 2);
    buffer_.erase(0, cr + 2);
    return message;
  }

  std::ostream &operator<<(std::ostream &os, const Buffer &obj)
  {
    os << obj.GetBuffer();
    return os;
  }
} // namespace irc
