#pragma once
#include <Irrlicht/irrlicht.h>

#include "Networking/ClientConnection.h"
#include "MazeGenerator.h"
#include "OpenAL\OpenALListener.h"
#include "Player.h"
#include "Audio\PlayerAudioEmitter.h"
#include "EventManager.h"
#include "Flag.h"

namespace Confus
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
        /// <summary>
        /// MazeGenerator that hasa accesible maze
        /// </summary>
        MazeGenerator m_MazeGenerator;
        /// <summary>
        /// The OpenAL listener that is attached to the camera.
        /// </summary>
        OpenALListener m_Listener;
        EventManager m_EventManager;
        /// <summary>
        /// The Players to test with.
        /// </summary>
        Player m_PlayerNode;
		Player m_SecondPlayerNode;
        /// <summary>
        /// The Blue Flag.
        /// </summary>
        Flag m_BlueFlag;
        /// <summary>
        /// The Red Flag.
        /// </summary>
        Flag m_RedFlag;
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
        std::unique_ptr<Networking::ClientConnection> m_Connection;

    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="Game"/> class.
        /// </summary>
        Game();
        /// <summary>
        /// Finalizes an instance of the <see cref="Game"/> class.
        /// </summary>
        virtual ~Game() = default;

        /// <summary>
        /// Starts the game and gameloop
        /// </summary>
        void run();
    private:
        /// <summary>
        /// Processes the triangle selectors.
        /// </summary>
        void processTriangleSelectors();
        irr::scene::IMetaTriangleSelector* processLevelMetaTriangles();        
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
    };
}