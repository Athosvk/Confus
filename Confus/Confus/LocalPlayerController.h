#pragma once
#include <RakNet/BitStream.h>

#include "Networking/ClientConnection.h"

namespace ConfusShared
{
	class EventManager;
	class Player;
}

namespace Confus
{
	class LocalPlayerController
	{
	private:
#pragma pack(push, 1)
		struct InputState
		{
			bool ForwardPressed = false;
			bool BackwardPressed = false;
			bool LeftPressed = false;
			bool RightPressed = false;
		} m_InputState;
#pragma pack(pop)

		ConfusShared::Player& m_Player;
		Networking::ClientConnection& m_Connection;
		long long m_ID;

	public:
		LocalPlayerController(ConfusShared::Player& a_Player, Networking::ClientConnection& m_Connection);
		
		void handleInput(ConfusShared::EventManager& a_EventManager);
		void fixedUpdate();
	private:
		void resetInputState();
	};
}
