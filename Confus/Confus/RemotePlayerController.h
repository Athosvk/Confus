#pragma once
namespace RakNet
{
	class BitStream;
}

namespace ConfusShared
{
	class Player;
}

namespace Confus
{
	namespace Networking
	{
		class ClientConnection;
	}
	
	/// <summary>
	/// Represents a player that is controlled by a remote server object, used so that the state of the server
	/// (position, rotation) is up to date with the local player and player actions can be re-executed locally
	/// </summary>
	/// <remarks>
	/// Separated from the player logic, so that we can share the player class between the server and 
	/// client side, avoiding code duplication.
	/// </remarks>
	class RemotePlayerController
	{
	private:		
		/// <summary>The player instance the remote instance is controlling in the local gameworld</summary>
		ConfusShared::Player& m_Player;		
		/// <summary>The connection to the server that is sending input for the player controlled</summary>
		Networking::ClientConnection& m_Connection;
	public:		
		/// <summary>Initializes a new instance of the <see cref="RemotePlayerController"/> class.</summary>
		/// <param name="a_Player">The player instance the remote instance is controlling in the local gameworld</param>
		/// <param name="a_Connection">The connection to the server that is sending input ofr the player controlled</param>
		RemotePlayerController(ConfusShared::Player& a_Player, Networking::ClientConnection& a_Connection);

	private:		
		/// <summary>Synchronizes the state of the player with that of the player on the server, so that both are up to date</summary>
		/// <param name="a_Data">The packet data, containing the state parameters of the player</param>
		void synchronizeState(RakNet::BitStream& a_Data) const;
	};
}