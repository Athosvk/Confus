#pragma once
#include <Irrlicht/irrlicht.h>

#include "Networking/ClientConnection.h"
#include "MazeGenerator.h"
#include "OpenAL\OpenALListener.h"
#include "Player.h"
#include "Audio\PlayerAudioEmitter.h"
#include "EventManager.h"
#include "Flag.h"
#include "RespawnFloor.h"
#include "GUI.h"
#include "ClientTeamScore.h"
#include "BaseGame.h"
#include "../ConfusShared/Physics/PhysicsWorld.h"

namespace Confus
{    
    /// <summary> 
    /// The Game instance itself, running the game loop. It ties the objects in
    /// the Game to the Irrlicht instance, so that these can communicate through this
    /// with the active Irrlicht instance 
    /// </summary>
    class Game : public BaseGame
    {
    private:
        /// <summary>
		/// <summary> The currently active physics world </summary>
		Physics::PhysicsWorld m_PhysicsWorld;
        /// MazeGenerator that hasa accesible maze
        /// </summary>
        MazeGenerator m_MazeGenerator;
        /// <summary>
        /// The OpenAL listener that is attached to the camera.
        /// </summary>
        OpenALListener m_Listener;
		/// <summary>
		/// The GUI for the Player
		/// </summary>
		GUI m_GUI;
        /// <summary>
        /// The Players to test with.
        /// </summary>
        Player m_PlayerNode;
        RespawnFloor m_RedRespawnFloor;
        RespawnFloor m_BlueRespawnFloor;

        std::vector<Player*> m_PlayerArray;
        /// <summary>
        /// The Blue Flag.
        /// </summary>
        Flag m_BlueFlag;
        /// <summary>
        /// The Red Flag.
        /// </summary>
        Flag m_RedFlag;
        irr::scene::ISceneNode* m_LevelRootNode;
		/// <summary>
		/// The connection as a client to the server that we are currently connected to
		/// </summary>
		std::unique_ptr<Networking::ClientConnection> m_Connection;

    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="Game" /> class.
        /// </summary>
        /// <param name="a_Device">The a_ device.</param>
        /// <param name="a_EventManager">The a_ event manager.</param>
        Game(irr::IrrlichtDevice* a_Device, EventManager* a_EventManager);

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
        /// <summary> Updates the (absolute) transformations of all the scene nodes recursively downwards </summary>
        void updateSceneTransformations();


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

        void removePlayer(RakNet::Packet* a_Data);

        // Inherited via BaseGame
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