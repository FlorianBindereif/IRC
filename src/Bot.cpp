#include <cstdlib>
#include <sstream>
#include "../inc/Bot.hpp"
#include "../inc/Print.hpp"

namespace irc
{
	static int RandomCard()
	{ return (std::rand() % 9) + 2;	}

	static std::string PlayerDrawsACard(ClientConnection* client)
	{
		std::stringstream output;
		short card = RandomCard();
		client->hands.player_hand += card;
		output << client->user.nick << " is drawing a card: " << card << "\n";
		return output.str();
	}

	static std::string DealerDrawsACard(ClientConnection* client)
	{
		std::stringstream output;
		short card = RandomCard();
		client->hands.dealer_hand += card;
		output << "The Dealer is drawing a card: " << card << "\n";
		return output.str();
	}

	std::string BlackJackBot(ClientConnection* client, Message& message)
	{
		std::stringstream output;
		if (message.trailing == "!Blackjack")
		{
			if (client->hands.player_hand == 0)
			{
				output << PlayerDrawsACard(client);
				output << PlayerDrawsACard(client);
				output << DealerDrawsACard(client);
			}
			output << "Your Score is: " << client->hands.player_hand << "\n";
			output << "!Hit or !Stay? \n";
		}
		else if (message.trailing == "!Hit" && client->hands.player_hand < 21)
		{
			output << PlayerDrawsACard(client);
			output << "Your Score is: " << client->hands.player_hand << "\n";
			output << "!Hit or !Stay? \n";
		}
		if (message.trailing == "!Stay" || client->hands.player_hand >= 21)
		{
			if (client->hands.player_hand > 21)
				output << "You bust! Dealer wins!\n";
			else
			{
				while (client->hands.dealer_hand < 17 || client->hands.dealer_hand < client->hands.player_hand)
					output << DealerDrawsACard(client);
				output << "the Dealers Score is: " << client->hands.dealer_hand << "\n";
				if (client->hands.dealer_hand > 21)
					output << "Dealer busts! You win!\n";
				else if (client->hands.player_hand > client->hands.dealer_hand)
					output << "You win! Congratulations!\n";
				else if (client->hands.player_hand < client->hands.dealer_hand)
					output << "Dealer wins! Play again!\n";
				else
					output << "Its a tie! Play again!\n";
			}
			client->hands.player_hand = 0;
			client->hands.dealer_hand = 0;
		}
		return output.str();
	}
}