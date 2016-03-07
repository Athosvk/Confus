#pragma once
#include <Irrlicht/irrlicht.h>

#include "Player.h"
#include "EventManager.h"

namespace Confus
{
    class Game
    {
    private:
        static const double FixedUpdateInterval;
        static const double MaxFixedUpdateInterval;

        irr::IrrlichtDevice* m_Device;
        EventManager m_EventManager;
        Player m_PlayerNode;

        double m_FixedUpdateTimer = 0.0;
        double m_DeltaTime = 0.0;
        irr::u32 m_PreviousTicks = 0;
        irr::u32 m_CurrentTicks = 0;

    public:
        Game();
        virtual ~Game() = default;

        void run();
    private:
        void handleInput();
        void update();
        void processFixedUpdates();
        void fixedUpdate();
        void render();
    };
}
