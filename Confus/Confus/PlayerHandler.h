#pragma once
#include <memory>
#include <map>

#include "LocalPlayerController.h"
#include "RemotePlayerController.h"
#include "../ConfusShared/Player.h"

namespace ConfusShared
{
	namespace Physics
	{
		class PhysicsWorld;
	}
}

namespace RakNet
{
	struct Packet;
}

namespace Confus
{
	namespace Audio
	{
		class AudioManager;
		class PlayerAudioEmitter;
	}
	namespace Networking
	{
		class ClientConnection;
	}

	class PlayerHandler
	{
	private:
		/// <summary>
		/// Represents the instances of objects that is used for every single player instance, which are tied
		/// together so that they can be update as pairs and can be removed at once when a player leaves 
		/// the game
		/// </summary>
		struct PlayerConstruct
		{
			/// <summary>Initializes a new instance of the <see cref="PlayerPair" /> struct.</summary>
			/// <param name="a_Player">The player.</param>
			/// <param name="a_AudioEmitter">The audio emitter.</param>
			/// <param name="a_Connection">The connection to the server, used for constructing a remote controller</param>
			PlayerConstruct(ConfusShared::Player* a_Player, std::unique_ptr<Audio::PlayerAudioEmitter> a_AudioEmitter,
				Networking::ClientConnection& a_Connection);

			/// <summary>The player instance, mirrored from the server</summary>
			ConfusShared::Player* Player;
			/// <summary>The audio emitter, playing audio for the associated player</summary>
			std::unique_ptr<Audio::PlayerAudioEmitter> AudioEmitter;
			/// <summary>
			/// The player controller that ensures synchronization between the local 
			/// instantiations of the players and those on the server
			/// </summary>
			std::unique_ptr<RemotePlayerController> PlayerController;
		};
		private:
			/// <summary>
			/// The players in the game world, indexed by their id (primarily ours)
			/// so that we can look them up easily for updates, removals etc.
			/// </summary>
			std::map<long long, PlayerConstruct> m_Players;

			/// <summary>
			/// The Players to test with.
			/// </summary>
			ConfusShared::Player m_PlayerNode;

			/// <summary>
			/// The controller controlling the player instance of the client, so that 
			/// inputs can be sent to the server
			/// </summary>
			std::unique_ptr<LocalPlayerController> m_PlayerController;
			
			/// <summary>
			/// A reference to the audio manager.
			/// </summary>
			Audio::AudioManager& m_AudioManager;

			/// <summary>
			/// A reference to the audio manager.
			/// </summary>
			ConfusShared::Physics::PhysicsWorld& m_PhysicsWorld;

			/// <summary>
			/// A reference to the irr device.
			/// </summary>
			irr::IrrlichtDevice* m_Device;

			/// <summary>
			/// A reference to the clientconnection.
			/// </summary>
			Confus::Networking::ClientConnection* m_Connection = nullptr;
			
	public:
		PlayerHandler(irr::IrrlichtDevice* a_Device, ConfusShared::Physics::PhysicsWorld& a_PhysicsWorld,
			Audio::AudioManager& a_AudioManager);
		void setConnection(Confus::Networking::ClientConnection* a_Connection);
		void update();
		void handleInput(ConfusShared::EventManager* a_EventManager) const;
		void fixedUpdate() const;
		ConfusShared::Player* getMainPlayer();
	private:
		/// <summary>
		/// Creates a new Player object for this user, this player will be regarded as THEIR player.
		/// </summary>
		void addOwnPlayer(RakNet::Packet* a_Data);
		/// <summary>
		/// Creates a new Player object for a different user that just joined.
		/// </summary>
		void addOtherPlayer(RakNet::Packet* a_Data);
		/// <summary>
		/// Updates positions and rotations of all other players.
		/// </summary>
		void updateOtherPlayer(RakNet::Packet* a_Data);
		/// <summary>
		/// Updates health of all players
		/// </summary>
		void updateHealth(RakNet::Packet* a_Data);
		/// <summary>
		/// Removes a player object.
		/// </summary>
		void removePlayer(RakNet::Packet* a_Data);
	};

}
