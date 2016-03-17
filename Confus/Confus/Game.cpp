#include <Irrlicht/irrlicht.h>
#include <sstream>

#include "Game.h"
#include "Player.h"
#include "EventManager.h"

namespace Confus
{
    const double Game::FixedUpdateInterval = 0.02;
    const double Game::MaxFixedUpdateInterval = 0.1;

    Game::Game()
        : m_Device(irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_OPENGL)),
        m_MoveableWall(m_Device, irr::core::vector3df(-30.0f, 0.0f, 0.0f),
            irr::core::vector3df(-30.f, -200.f, 0.0f)),
        m_PlayerNode(m_Device)

    {
    }
    void Game::run()
    {
        auto sceneManager = m_Device->getSceneManager();
        sceneManager->loadScene("Media/IrrlichtScenes/Bases.irr");
        m_Device->getCursorControl()->setVisible(false);

        m_Device->setEventReceiver(&m_EventManager);

        while(m_Device->run())
        {
            handleInput();
            update();
            processFixedUpdates();
            render();
        }
    }

    void Game::handleInput()
    {
        m_PlayerNode.handleInput(m_EventManager);
    }

    void Game::update()
    {
        m_PreviousTicks = m_CurrentTicks;
        m_CurrentTicks = m_Device->getTimer()->getTime();
        m_DeltaTime = (m_CurrentTicks - m_PreviousTicks) / 1000.0;
    }

    void Game::processFixedUpdates()
    {
        m_FixedUpdateTimer += m_DeltaTime;
        m_FixedUpdateTimer = irr::core::min_(m_FixedUpdateTimer, MaxFixedUpdateInterval);
        while(m_FixedUpdateTimer >= FixedUpdateInterval)
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
        m_Device->getVideoDriver()->beginScene(true, true, irr::video::SColor(255, 100, 101, 140));
        m_Device->getSceneManager()->drawAll();
        m_Device->getGUIEnvironment()->drawAll();
        m_Device->getVideoDriver()->endScene();
    }
}