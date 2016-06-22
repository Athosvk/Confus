#pragma once
#include <RakNet/MessageIdentifiers.h>

namespace ConfusShared
{
	namespace Networking
	{
		/// <summary> The type of packet </summary>
		enum class EPacketType : unsigned char
		{
			/// <summary>Represents a simple message to notify a client or server</summary>
			Message = 1 + ID_USER_PACKET_ENUM,
			/// <summary>Sent when the player itself has been allowed into the server's game instance </summary>
			MainPlayerJoined = 2 + ID_USER_PACKET_ENUM,
			/// <summary>Sent to the clients when a new player joined the game</summary>
			OtherPlayerJoined = 3 + ID_USER_PACKET_ENUM,
			/// <summary>Sent when a player has left, so that the clients can update the local state of the player</summary>
			PlayerLeft = 4 + ID_USER_PACKET_ENUM,
			/// <summary>Sent so that the clietns can update their local representations of the players</summary>
			UpdatePosition = 5 + ID_USER_PACKET_ENUM,
			/// <summary>Sent so that the client has the most up to date score and can update its own GUI accordingly</summary>
			ScoreUpdate = 6 + ID_USER_PACKET_ENUM,
			/// <summary>
			/// Sent when a client has initiated an attack, so that the server can invoke it locally
			/// and 'send' the result of the attack back and broadcast it to other clients as well
			/// </summary>
			PlayerAttack = 7 + ID_USER_PACKET_ENUM,
			/// <summary>
			/// Sent when the maze is nearing the change of seed, so that clients can prepare an update
			/// of their own maze representation
			/// </summary>
			MazeChange = 8 + ID_USER_PACKET_ENUM,
			/// <summary> Sent to notify the server of the player's inputs, so that it can update the player state</summary>
			Player = 9 + ID_USER_PACKET_ENUM,
			/// <summary> Sent when there has been an update in health of a player instance, so that 
			/// where needed, a player can update its visual representation for the GUI
			/// </summary>
            UpdateHealth = 10 + ID_USER_PACKET_ENUM,
            /// <summary>Sent when the game has end, so that clients can present the winning screen and show the winning team</summary>
            EndOfGame = 11 + ID_USER_PACKET_ENUM,
            
            RedFlag = 12 + ID_USER_PACKET_ENUM,
            BlueFlag = 13 + ID_USER_PACKET_ENUM,
			JoinSync = 14 + ID_USER_PACKET_ENUM,
		};
	}
}