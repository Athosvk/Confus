#pragma once
#include <Irrlicht/irrlicht.h>

#include "MoveableWall.h"

namespace Confus
{
    class Game
    {
    private:
        static const double FixedUpdateInterval;
        static const double MaxFixedUpdateInterval;

        irr::IrrlichtDevice* m_Device;
        irr::scene::ISceneManager* m_SceneManager;
        irr::video::IVideoDriver* m_VideoDriver;
        irr::gui::IGUIEnvironment* m_GuiEnvironment;

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
