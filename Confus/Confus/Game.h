#pragma once
#include <Irrlicht/irrlicht.h>

#include "Networking/ClientConnection.h"
#include "MazeGenerator.h"
#include "Audio/OpenAL\OpenALListener.h"
#include "Player.h"
#include "Audio\PlayerAudioEmitter.h"
#include "EventManager.h"
#include "Flag.h"
#include "RespawnFloor.h"
#include "GUI.h"
#include "ClientTeamScore.h"
#include "Audio/AudioManager.h"
#include "../ConfusShared/Physics/PhysicsWorld.h"
#include "Announcer.h"

namespace Confus
{    
    /// <summary> 
    /// The Game instance itself, running the game loop. It ties the objects in
    /// the Game to the Irrlicht instance, so that these can communicate through this
    /// with the active Irrlicht instance 
    /// </summary>
    class Game
    {
	public:
		/// <summary>
		/// The maximum score used to determine if someone has won
		/// </summary>
		static const int MaxScore;
    private:
        /// <summary>
        /// The rate at which fixed updates are carried out
        /// </summary>
        static const double FixedUpdateInterval;
        /// <summary>
        /// The interval to clamp to if the delay between sequential fixed updates is too long
        /// </summary>
        static const double MaxFixedUpdateInterval;

        /// <summary>
        /// The instance of the IrrlichtDevice
		/// Statics are avoided to make code clearer, hence this is not a static
        /// </summary>
        irr::IrrlichtDevice* m_Device;
		/// <summary> The currently active physics world </summary>
		Physics::PhysicsWorld m_PhysicsWorld;
        /// <summary>
        /// The OpenAL listener that is attached to the camera.
        /// </summary>
        Audio::OpenAL::OpenALListener m_Listener;
		Audio::AudioManager m_AudioManager;
		/// <summary>
		/// MazeGenerator that hasa accesible maze
		/// </summary>
		MazeGenerator m_MazeGenerator;

        EventManager m_EventManager;
		/// <summary>
		/// The GUI for the Player
		/// </summary>
		GUI m_GUI;
        /// <summary>
        /// The Players to test with.
        /// </summary>
        Player m_PlayerNode;


        std::vector<Player*> m_PlayerArray;
        /// <summary>
        /// The Blue Flag.
        /// </summary>
        Flag m_BlueFlag;
        /// <summary>
        /// The Red Flag.
        /// </summary>
        Flag m_RedFlag;

		Announcer m_Announcer;
		RespawnFloor m_RedRespawnFloor;
		RespawnFloor m_BlueRespawnFloor;

        /// <summary>
        /// The delay between the last and future fixed update
        /// </summary>
        double m_FixedUpdateTimer = 0.0;
        /// <summary>
		/// The time interval between the last update and the second-last
        /// </summary>
        double m_DeltaTime = 0.0;
        /// <summary>
		/// The total elapsed game ticks in milliseconds in the last frame
        /// </summary>
        irr::u32 m_PreviousTicks = 0;
        /// <summary>
        /// The total elapsed game ticks in the current frame
        /// </summary>
        irr::u32 m_CurrentTicks = 0;
        irr::scene::ISceneNode* m_LevelRootNode;
		/// <summary>
		/// The connection as a client to the server that we are currently connected to
		/// </summary>
		std::unique_ptr<Networking::ClientConnection> m_Connection;

		
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="Game"/> class.
        /// </summary>
        Game(irr::core::dimension2d<irr::u32> a_Resolution);
        /// <summary>
        /// Finalizes an instance of the <see cref="Game"/> class.
        /// </summary>
        virtual ~Game();

        /// <summary>
        /// Starts the game and gameloop
        /// </summary>
        void run();
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
        /// Updates the state of the objects in the game
        /// </summary>
        void update();
        /// <summary>
        /// Runs a set of fixed update calls based on the tim elapsed since the last
        /// </summary>
        void processFixedUpdates();
        /// <summary>
        /// Updates the state of objects that require frame-rate independence
        /// </summary>
        void fixedUpdate();
        /// <summary>
        /// Renders the objects in the game
        /// </summary>
        void render();

        /// <summary>
        /// Creates a new Player object for this user, this player will be regarded as THEIR player.
        /// </summary>
        void addOwnPlayer(RakNet::BitStream* a_Data);
        /// <summary>
        /// Creates a new Player object for a different user that just joined.
        /// </summary>
        void addOtherPlayer(RakNet::BitStream* a_Data);
        void removePlayer(RakNet::BitStream* a_Data);

		/// <summary> Updates the (absolute) transformations of all the scene nodes recursively downwards </summary>
		void updateSceneTransformations();
    };
}
