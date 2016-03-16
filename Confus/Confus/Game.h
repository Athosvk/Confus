#pragma once
#include <Irrlicht/irrlicht.h>

#include "MoveableWall.h"
#include "Player.h"
#include "EventManager.h"

namespace Confus
{
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
        /// </summary>
        irr::IrrlichtDevice* m_Device;
        /// <summary>
        /// The MoveableWall as test object
        /// </summary>
        MoveableWall m_MoveableWall;
        EventManager m_EventManager;
        Player m_PlayerNode;
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