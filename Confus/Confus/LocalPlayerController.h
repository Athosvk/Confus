#pragma once
#include "Networking/ClientConnection.h"
#include "../ConfusShared/PlayerInputState.h"

namespace ConfusShared
{
	class EventManager;
	class Player;
}

namespace Confus
{	
	/// <summary>
	/// Handles the input of the local player, so that it can be forwarded to the server
	/// and optionally applied to the player to perform prediction
	/// </summary>
	class LocalPlayerController
	{
	private:		
		/// <summary>The player to control</summary>
		ConfusShared::Player& m_Player;		
		/// <summary>The connection with the server to send the player state to, so that it can update the player remotely</summary>
		Networking::ClientConnection& m_Connection;		
		/// <summary>The current state of the input parameters for the player, so it can be sent to the server</summary>
		ConfusShared::PlayerInputState m_InputState;

	public:		
		/// <summary>Initializes a new instance of the <see cref="LocalPlayerController"/> class.</summary>
		/// <param name="a_Player">The player this controller is controlling</param>
		/// <param name="m_Connection">The connection with the server to send the player state to </param>
		LocalPlayerController(ConfusShared::Player& a_Player, Networking::ClientConnection& m_Connection);
				
		/// <summary>Handles the input to change the state that is sent to the server</summary>
		/// <param name="a_EventManager">The event manager to check the input state of</param>
		void handleInput(ConfusShared::EventManager& a_EventManager);

        /// <summary>Sends the current player state to the server, so that the player can be updated remotely</summary>
        void update();
	private:		
		/// <summary>Resets the input state back to normal, so that newly pressed keys are registered</summary>
		void resetInputState();
	};
}
