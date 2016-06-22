#pragma once
#include <Irrlicht/irrlicht.h>

#include "Networking/Connection.h"
#include "../ConfusShared/MazeManager.h"
#include "../ConfusShared/Player.h"
#include "../ConfusShared/Flag.h"
#include "../ConfusShared/Physics/PhysicsWorld.h"
#include "TeamScore.h"
#include "RemoteInputReceiver.h"
#include "RemoteFlagUpdater.h"

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
		/// <summary>
		/// Represents a combination of a Player and its RemoteInputReceiver, allowing it to bind input
		/// to that particular player. Allows for easy removal of this combination once the player leaves
		/// </summary>
		struct PlayerPair
		{			
			/// <summary>The player representing a client's interface</summary>
			ConfusShared::Player* Player;			
			/// <summary>
			/// The input receiver, registering the input that the client forwarded
			/// to the server 
			/// </summary>
			std::unique_ptr<RemoteInputReceiver> Receiver;
		public:			
			/// <summary>Initializes a new instance of the <see cref="PlayerPair"/> struct.</summary>
			/// <param name="a_Player">The player</param>
			/// <param name="a_Connection">The connection opened by the server, to construct an input receiver</param>
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
		static const double BroadcastInterval;
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
        ConfusShared::MazeManager m_MazeManager;		
		/// <summary>The players in the GameWorld, indexed by their ID so that we can find individuals easily</summary>
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
		double m_BroadcastTimer = 0.0;
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
		/// <summary>
		/// The level root node, under which all colliders fromt he scene file are placed
		/// so that we can selectively create colliders for those nodes
		/// </summary>
		irr::scene::ISceneNode* m_LevelRootNode = nullptr;
        /// <summary> Team Score Manager </summary>
        TeamScore m_TeamScoreManager;

        std::unique_ptr<RemoteFlagUpdater> m_RedFlagUpdater;
        std::unique_ptr<RemoteFlagUpdater> m_BlueFlagUpdater;
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

		void processBroadcasts();

		void broadcastUpdates();
        /// <summary>
        /// Runs a set of fixed update calls based on the time elapsed since the last frame
        /// </summary>
        void processFixedUpdates();

		void initializeLevelColliders();

        /// <summary> Updates the state of objects that require frame-rate independence </summary>
        void fixedUpdate();
		
		/// <summary>Adds a player from the given data, so that it can be synced with the remote players</summary>
		/// <param name="a_Data">The packet data containing the information needed to construct a player object</param>
		void addPlayer(RakNet::Packet* a_Data);
		
		/// <summary>Removes a player that has disconnected, so that his player instance is no longer in the game world</summary>
		/// <param name="a_Data">The packet containing the information needed to remove the player</param>
		void removePlayer(RakNet::Packet* a_Data);
		
		/// <summary> Sends the new position and rotation information for each player so they can be updated locally</summary>
		void sendPlayerUpdates();
		
		/// <summary>Sends the new health of </summary>
		/// <param name="a_HitType">Type of the a_ hit.</param>
		/// <param name="a_Player">The a_ player.</param>
		void updateHealth(EHitIdentifier a_HitType, ConfusShared::Player* a_Player) const;

        /// <summary>
        /// Broadcast a maze change
        /// </summary>
        void broadcastMazeChange(int a_Seed) const;

        /// <summary> Reset game </summary>
        void resetGame();
		
		/// <summary> 
		/// Updates the (absolute) transformations of all the scene nodes recursively downwards to make
		/// sure that the absolute positions have been updated once the physics world requests them
		/// </summary>
		void updateSceneTransformations() const;
    };
}