#pragma once
#include <Irrlicht/irrlicht.h>

#include "Networking/ClientConnection.h"
#include "../ConfusShared/MazeGenerator.h"
#include "Audio/OpenAL/OpenALListener.h"
#include "Audio/PlayerAudioEmitter.h"
#include "../ConfusShared/EventManager.h"
#include "../ConfusShared/Flag.h"
#include "../ConfusShared/RespawnFloor.h"
#include "GUI.h"
#include "ClientTeamScore.h"
#include "../ConfusShared/BaseGame.h"
#include "Audio/AudioManager.h"
#include "../ConfusShared/Physics/PhysicsWorld.h"
#include "Announcer.h"
#include "LocalPlayerController.h"
#include "RemotePlayerController.h"

namespace Confus
{    
    /// <summary> 
    /// The Game instance itself, running the game loop. It ties the objects in
    /// the Game to the Irrlicht instance, so that these can communicate through this
    /// with the active Irrlicht instance 
    /// </summary>
    class Game : public ConfusShared::BaseGame
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
			
			/// <summary>
			/// The player instance that is either the player of this instance or that of a 
			/// different client connected to the server, so that we can update values such as position
			/// and animation state of all Player instances
			/// </summary>
			ConfusShared::Player* Player;
			/// <summary>
			/// The audio emitter, playing audio for the associated player, placed along with the player instance
			/// so that they can be removed in association at once
			/// </summary>
			std::unique_ptr<Audio::PlayerAudioEmitter> AudioEmitter;			
			/// <summary>
			/// The player controller that ensures synchronization between the local 
			/// instantiations of the players and those on the server, placed along with the player instance
			/// so that they can be removed in association at once
			/// </summary>
			std::unique_ptr<RemotePlayerController> PlayerController;
		};
	public:
		/// <summary>
		/// The maximum score used to determine if someone has won
		/// </summary>
		static const int MaxScore;
    private:
        /// <summary>
		/// <summary> The currently active physics world </summary>
		ConfusShared::Physics::PhysicsWorld m_PhysicsWorld;
        /// The OpenAL listener that is attached to the camera.
        /// </summary>
        Audio::OpenAL::OpenALListener m_Listener;        
        /// <summary>
        /// The class audio manager that handles the opanal sources for you.
        /// </summary>
        Audio::AudioManager m_AudioManager;

		/// <summary>
		/// MazeGenerator that hasa accesible maze
		/// </summary>
		ConfusShared::MazeGenerator m_MazeGenerator;

		/// <summary>
		/// The GUI for the Player
		/// </summary>
		GUI m_GUI;
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
		/// The players in the game world, indexed by their id (primarily ours)
		/// so that we can look them up easily for updates, removals etc.
		/// </summary>
		std::map<long long, PlayerConstruct> m_Players;
        /// <summary>
        /// The Blue Flag.
        /// </summary>
        ConfusShared::Flag m_BlueFlag;
        /// <summary>
        /// The Red Flag.
        /// </summary>
        ConfusShared::Flag m_RedFlag;
		
		/// <summary>The announcer playing sounds related to capture events and such</summary>
		Announcer m_Announcer;
		/// <summary>The red team's respawn floor</summary>
		ConfusShared::RespawnFloor m_RedRespawnFloor;
		/// <summary>The blue team's respawn floor</summary>
		ConfusShared::RespawnFloor m_BlueRespawnFloor;

		/// <summary>
		/// The level root node, under which all colliders fromt he scene file are placed
		/// so that we can selectively create colliders for those nodes
		/// </summary>
        irr::scene::ISceneNode* m_LevelRootNode;
		/// <summary>
		/// The connection as a client to the server that we are currently connected to,
		/// used for sending packets over the connection to the server and 
		/// </summary>
		std::unique_ptr<Networking::ClientConnection> m_Connection;
		/// <summary>The sound played once the maze changes seeds</summary>
		Audio::Sound m_MazeChangedSound;
		/// <summary>The client side representation of the team score</summary>
		ClientTeamScore m_ClientScore;

    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="Game" /> class.
        /// </summary>
        /// <param name="a_Device">The a_ device.</param>
        /// <param name="a_EventManager">The a_ event manager.</param>
        Game(irr::IrrlichtDevice* a_Device, ConfusShared::EventManager* a_EventManager);

        /// <summary>
        /// Finalizes an instance of the <see cref="Game"/> class.
        /// </summary>
        virtual ~Game();

        /// <summary>
        /// Resets the game and gameloop
        /// </summary>
        void reset();
    private:
        /// <summary> Creates all the colliders for the level </summary>
        void initializeLevelColliders();
        /// <summary>
        /// Initializes the connection to the server.
        /// </summary>
        void initializeConnection();
        /// <summary>
        /// Processes the input data
        /// </summary>
        void handleInput();

        /// <summary> 
		/// Updates the (absolute) transformations of all the scene nodes recursively downwards to make
		/// sure that the absolute positions have been updated once the physics world requests them
		/// </summary>
        void updateSceneTransformations() const;

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
		
		/// <summary>Removes the player instance from the local game instance</summary>
		/// <param name="a_Data">The data containing the information of the player needed to remove the player</param>
		void removePlayer(RakNet::Packet* a_Data);
		
		/// <summary>Denies the connection</summary>
		/// <param name="a_Data">The data containing why the connection was refused</param>
		void denyConnection(RakNet::Packet* a_Data);
		
		/// <summary>Starts this instance.</summary>
		virtual void start() override;

        /// <summary>
        /// Updates the state of the objects in the game
        /// </summary>
        virtual void update() override;

        /// <summary>
        /// Updates the state of objects that require frame-rate independence
        /// </summary>
        virtual void fixedUpdate() override;

		/// <summary>Ends this instance.</summary>
		virtual void end() override;

		/// <summary>Renders the meshes and other objects in the game</summary>
		virtual void render() override;
	};
}
