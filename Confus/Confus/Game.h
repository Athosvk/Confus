#pragma once
#include <Irrlicht/irrlicht.h>

#include "Networking/ClientConnection.h"
#include "../ConfusShared/MazeManager.h"
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
#include "PlayerHandler.h"
#include "LocalPlayerController.h"
#include "RemoteFlagController.h"
#include "CameraController.h"

namespace Confus
{    
    /// <summary> 
    /// The Game instance itself, running the game loop. It ties the objects in
    /// the Game to the Irrlicht instance, so that these can communicate through this
    /// with the active Irrlicht instance 
    /// </summary>
    class Game : public ConfusShared::BaseGame
    {
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
		ConfusShared::MazeManager m_MazeManager;
		
		/// <summary>
		/// The Player handler that handles the own player and the newly joined ones.
		/// </summary>
		PlayerHandler m_PlayerHandler;

		/// <summary>
		/// The GUI for the Player
		/// </summary>
		GUI m_GUI;
		
        std::unique_ptr<RemoteFlagController> m_RedFlagController;
        std::unique_ptr<RemoteFlagController> m_BlueFlagController;
        /// <summary>
        /// The Blue Flag.
        /// </summary>
        ConfusShared::Flag m_BlueFlag;
        /// <summary>
        /// The Red Flag.
        /// </summary>
        ConfusShared::Flag m_RedFlag;

		Announcer m_Announcer;
		ConfusShared::RespawnFloor m_RedRespawnFloor;
		ConfusShared::RespawnFloor m_BlueRespawnFloor;

		/// <summary>
		/// The level root node, under which all colliders fromt he scene file are placed
		/// so that we can selectively create colliders for those nodes
		/// </summary>
        irr::scene::ISceneNode* m_LevelRootNode;
		/// <summary>
		/// The connection as a client to the server that we are currently connected to
		/// </summary>
		std::unique_ptr<Networking::ClientConnection> m_Connection;
		Audio::Sound m_MazeChangedSound;
		ClientTeamScore m_ClientScore;
		std::unique_ptr<CameraController> m_Camera;

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

		void denyConnection(RakNet::Packet* a_Data);

        virtual void start() override;
        /// <summary>
        /// Updates the state of the objects in the game
        /// </summary>
        virtual void update() override;
        /// <summary>
        /// Updates the state of objects that require frame-rate independence
        /// </summary>
        virtual void fixedUpdate() override;
        virtual void end() override;
        virtual void render() override;
	};
}
