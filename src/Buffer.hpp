#include "../config.hpp"

namespace irc
{
	namespace buffer
	{
		class Buffer
		{
			private:
				char buffer[MESSAGE_LENGTH];
			public:
				fill();
		};
	}
}