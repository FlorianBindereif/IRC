#ifndef CLIENT_CONNECTION_HPP
#define CLIENT_CONNECTION_HPP

#include "Connection.hpp"

#define SERVINVIS 0b01
#define SERVOP 0b10

namespace irc
{
  struct Blackjack
  {
    short player_hand;
    short dealer_hand;
    Blackjack(short player_hand, short dealer_hand)
        : player_hand(player_hand), dealer_hand(dealer_hand) {}
  };

  class Message;

  class ClientConnection : public Connection
  {
  public:
    unsigned char client_mode;
    Buffer input_buffer;
    Buffer output_buffer;
    std::vector<std::string> channel_list;
    Blackjack hands;

  public:
    ClientConnection();
    ClientConnection(int fd);
    virtual ~ClientConnection();

  private:
    void SetChannelMode_(Message &message);
    void SetClientMode_(Message &message);
    std::string GetModeString_() const;
    std::string CleanModeString_(std::string &mode, std::string flag_string);

  public:
    void Receive();
    void Send();
    void CloseConnection();

  public:
    void ExecuteMessage(std::string command);
    void SendCapabilities(Message &message);
    void Authenticate(Message &message);
    void SetUsername(Message &message);
    void SetNickname(Message &message);
    void JoinChannel(Message &message);
    void SendPong(Message &message);
    void SetMode(Message &message);
    void SendNames(Message &message);
    void PartChannel(Message &message);
    void SendMessage(Message &message);
    void SendNotice(Message &message);
    void InviteClient(Message &message);
    void SetTopic(Message &message);
    void KickMember(Message &message);
    void MakeOperator(Message &message);
    std::string ExecuteBot(Message &message);
    void ShutdownServer(Message &message);
    void LeaveServer();
  };
} // namespace irc

#endif