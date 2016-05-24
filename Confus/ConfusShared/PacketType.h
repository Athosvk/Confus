#pragma once
#include <RakNet/MessageIdentifiers.h>

namespace ConfusShared
{
	namespace Networking
	{
		/// <summary> The type of packet </summary>
		enum class EPacketType : unsigned char
		{
			Message = 1 + ID_USER_PACKET_ENUM,
			MainPlayerJoined = 2 + ID_USER_PACKET_ENUM,
			OtherPlayerJoined = 3 + ID_USER_PACKET_ENUM,
			PlayerLeft = 4 + ID_USER_PACKET_ENUM,
			UpdatePosition = 5 + ID_USER_PACKET_ENUM,
			ScoreUpdate = 6 + ID_USER_PACKET_ENUM,
			PlayerAttack = 7 + ID_USER_PACKET_ENUM,
			MazeChange = 8 + ID_USER_PACKET_ENUM,
			Player = 9 + ID_USER_PACKET_ENUM,
			UpdateHealth = 10 + ID_USER_PACKET_ENUM,
			EndOfGame = 11 + ID_USER_PACKET_ENUM
		};
	}
}