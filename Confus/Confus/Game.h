#pragma once
#include <Irrlicht/irrlicht.h>

namespace Confus
{
    class Game
    {
    private:
        static const double FixedUpdateInterval;
        static const double MaxFixedUpdateInterval;

        irr::IrrlichtDevice* m_Device;

        double m_FixedUpdateTimer = 0.0;
        double m_DeltaTime = 0.0;

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
