#pragma once
#include <Irrlicht/irrlicht.h>

#include "Networking/Connection.h"
#include "../ConfusShared/MazeGenerator.h"
#include "../ConfusShared/Player.h"
#include "../ConfusShared/Flag.h"
#include "../ConfusShared/Physics/PhysicsWorld.h"
#include "TeamScore.h"
#include "RemoteInputReceiver.h"

namespace ConfusServer
{ 
    /// <summary> 
    /// The Game instance itself, running the game loop. It ties the objects in
    /// the Game to the Irrlicht instance, so that these can communicate through this
    /// with the active Irrlicht instance 
    /// </summary>
    class Game
    {
    private:
		struct PlayerPair
		{
			ConfusShared::Player* Player;
			std::unique_ptr<RemoteInputReceiver> Receiver;

		public:
			PlayerPair(ConfusShared::Player* a_Player, Networking::Connection& a_Connection);
		};

        /// <summary>
        /// The rate at which fixed updates are carried out
        /// </summary>
        static const double FixedUpdateInterval;
        /// <summary>
        /// The interval to clamp to if the delay between sequential fixed updates is too long
        /// </summary>
        static const double MaxFixedUpdateInterval;
		/// <summary>
		/// The interval at which packets queue before processed
		/// </summary>
		static const double ProcessPacketsInterval;
        /// <summary>
        /// The interval at which packets queue before processed
        /// </summary>
        static const double MazeChangeInterval;
        /// <summary>
        /// The delay the maze has before it changes after broadcasting
        /// </summary>
        static const double MazeDelay;

        /// <summary>
        /// The instance of the IrrlichtDevice
		/// Statics are avoided to make code clearer, hence this is not a static
        /// </summary>
        irr::IrrlichtDevice* m_Device;
		/// <summary>The physics world, allowing simulation of rigid bodies and colliders</summary>
		ConfusShared::Physics::PhysicsWorld m_PhysicsWorld;
        /// <summary>
        /// MazeGenerator that hasa accesible maze
        /// </summary>
        ConfusShared::MazeGenerator m_MazeGenerator;
        std::map<long long, PlayerPair> m_Players;
        /// <summary>
        /// The Blue Flag.
        /// </summary>
        ConfusShared::Flag m_BlueFlag;
        /// <summary>
        /// The Red Flag.
        /// </summary>
        ConfusShared::Flag m_RedFlag;
        /// <summary>
        /// The delay between the last and future fixed update
        /// </summary>
        double m_FixedUpdateTimer = 0.0;
		/// <summary>
		/// The delay between the last and future packet update
		/// </summary>
		double m_ConnectionUpdateTimer = 0.0;
        /// <summary>
		/// The time interval between the last update and the second-last
        /// </summary>
        double m_DeltaTime = 0.0;
        /// <summary>
        /// The time interval between the last update and the new maze update.
        /// </summary>
        double m_MazeTimer = 0.0;
        /// <summary>
		/// The total elapsed game ticks in milliseconds in the last frame
        /// </summary>
        irr::u32 m_PreviousTicks = 0;
        /// <summary>
        /// The total elapsed game ticks in the current frame
        /// </summary>
        irr::u32 m_CurrentTicks = 0;
        /// <summary> The connection to the clients of this server</summary>
        std::unique_ptr<Networking::Connection> m_Connection;
        irr::scene::ISceneNode* m_LevelRootNode = nullptr;
        /// <summary> Team Score Manager </summary>
        TeamScore m_TeamScoreManager;
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="Game"/> class.
        /// </summary>
        Game();
        /// <summary>
        /// Finalizes an instance of the <see cref="Game"/> class.
        /// </summary>
        virtual ~Game();

        /// <summary>
        /// Starts the game and gameloop
        /// </summary>
        void run();
    private:
        /// <summary>
        /// Opens the connections for clients.
        /// </summary>
        void initializeConnection();
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
		/// Processes the packets connection
		/// </summary>
		void processConnection();
	    void addPlayer(RakNet::Packet* a_Data);
        void removePlayer(RakNet::Packet* a_Data);
        void updatePlayers();
        void updateHealth(EHitIdentifier a_HitType, ConfusShared::Player* a_Player) const;
        /// <summary>
        /// Broadcast a maze change
        /// </summary>
        void broadcastMazeChange(int a_Seed) const;
        /// <summary> Reset game </summary>
        void resetGame();
    };
}