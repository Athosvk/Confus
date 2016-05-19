#pragma once
#include <RakNet/BitStream.h>

#include "Player.h"
#include "Networking/ClientConnection.h"

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

		Player& m_Player;
		Networking::ClientConnection& m_Connection;

	public:
		LocalPlayerController(Player& a_Player, Networking::ClientConnection& m_Connection);
		
		void handleInput(ConfusShared::EventManager& a_EventManager);
		void fixedUpdate();
	private:
		void resetInputState();
		void synchronizeState(RakNet::BitStream& a_Data);
	};
}
