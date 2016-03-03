#include <Irrlicht/irrlicht.h>
#include <sstream>

#include "Game.h"
#include "OpenALAudio.h"
#include "Player.h"

namespace Confus
{
    const double Game::FixedUpdateInterval = 0.02;
    const double Game::MaxFixedUpdateInterval = 0.1;

    Game::Game() : 
        m_Device(irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_OPENGL)),
        m_MoveableWall(m_Device, irr::core::vector3df(0.0f, 0.0f, 60.0f),
            irr::core::vector3df(0.f, -200.f, 60.0f))
    {
    }

    void Game::run()
    {
        m_SceneManager = m_Device->getSceneManager();
        m_VideoDriver = m_Device->getVideoDriver();
        m_GuiEnvironment = m_Device->getGUIEnvironment();

        auto camera = m_SceneManager->addCameraSceneNodeFPS();
        m_Device->getCursorControl()->setVisible(false);

        auto playerNode = Player(m_SceneManager);

        OpenALAudio sound;
        std::ostringstream oss;
        oss << "Played Sound: " << sound.PlayASound();

        while (m_Device->run())
        {
            handleInput();
            update();
            processFixedUpdates();
            render();
        }
    }

    void Game::handleInput()
    {
    }

    void Game::update()
    {
    }

    void Game::processFixedUpdates()
    {
        m_FixedUpdateTimer += m_DeltaTime;
        m_FixedUpdateTimer = irr::core::min_(m_FixedUpdateTimer, MaxFixedUpdateInterval);
        while (m_FixedUpdateTimer >= FixedUpdateInterval)
        {
            m_FixedUpdateTimer -= FixedUpdateInterval;
            fixedUpdate();
        }
    }

    void Game::fixedUpdate()
    {
        m_MoveableWall.fixedUpdate();
    }

    void Game::render()
    {
        m_VideoDriver->beginScene(true, true, irr::video::SColor(255, 100, 101, 140));
        m_SceneManager->drawAll();
        m_GuiEnvironment->drawAll();
        m_VideoDriver->endScene();
    }
}
