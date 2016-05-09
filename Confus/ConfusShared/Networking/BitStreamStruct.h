#pragma once
#include <RakNet\GetTime.h>
#include <Irrlicht\irrlicht.h>

namespace ConfusShared
{
	namespace Networking
	{				
		/// <summary>
		/// Player networking states
		/// </summary>
		enum class EPlayerState : unsigned char
		{
			Alive, CarryingFlag, HeavyAttacking, LightAttacking, Dead
		};

		/// <summary>
		/// Player information that gets networked
		/// </summary>
		struct  PlayerInfo
		{
			unsigned int playerID = 0u;
			irr::core::vector3df position = irr::core::vector3df(0.f);
			irr::core::vector3df rotation = irr::core::vector3df(0.f);
			EPlayerState newState;
			RakNet::Time stateChangeTime;
			int8_t playerHealth;
			bool forwardKeyPressed = false;
			bool backwardKeyPressed = false;
			bool leftKeyPressed = false;
			bool rightKeyPressed = false;
			bool leftMouseButtonPressed = false;
			bool rightMouseButtonPressed = false;
		};
	}
}