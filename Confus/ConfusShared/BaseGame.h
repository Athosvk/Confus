#pragma once
#include <Irrlicht/irrlicht.h>

namespace ConfusShared
{    
	class EventManager;

    /// <summary>
    /// Base Game class, scenes can use this to run a scene
    /// </summary>
    class BaseGame
    {
    protected:
        /// <summary>
        /// The rate at which fixed updates are carried out
        /// </summary>
        static const double FixedUpdateInterval;
        /// <summary>
        /// The interval to clamp to if the delay between sequential fixed updates is too long
        /// </summary>
        static const double MaxFixedUpdateInterval;
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
        /// <summary>
        /// The instance of the IrrlichtDevice
        /// Statics are avoided to make code clearer, hence this is not a static
        /// </summary>
        irr::IrrlichtDevice* m_Device;
        /// <summary> Will stop the game loop </summary>
        bool m_ShouldRun = true;		
		/// <summary>The active event manager, used for handling input events</summary>
		EventManager* m_EventManager;
    public:        
        /// <summary>
        /// Initializes a new instance of the <see cref="BaseGame"/> class.
        /// </summary>
        /// <param name="a_Device">The a_ device.</param>
        /// <param name="a_EventManager">The a_ event manager.</param>
        BaseGame(irr::IrrlichtDevice* a_Device, EventManager* a_EventManager);

        /// <summary>
        /// Starts the game and gameloop
        /// </summary>
        virtual void run();
    protected:

        /// <summary>
        /// Called at the start of the game.
        /// </summary>
        virtual void start() = 0;
        /// <summary>
        /// Updates the state of the objects in the game
        /// </summary>
        virtual void update() = 0;
        /// <summary>
        /// Updates the state of objects that require frame-rate independence
        /// </summary>
        virtual void fixedUpdate() = 0;
        /// <summary>
        /// Called at the end of the game.
        /// </summary>
        virtual void end() = 0;

        /// <summary>
        /// Runs a set of fixed update calls based on the tim elapsed since the last
        /// </summary>
        virtual void processFixedUpdates();
        /// <summary>
        /// Renders the objects in the game
        /// </summary>
        virtual void render();
    };
}

