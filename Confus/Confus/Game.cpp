#include <Irrlicht/irrlicht.h>
#include <sstream>

#include "Game.h"
#include "OpenALAudio.h"
#include "Player.h"
#include "EventManager.h"

namespace Confus
{
    const double Game::FixedUpdateInterval = 0.02;
    const double Game::MaxFixedUpdateInterval = 0.1;

    Game::Game()
        : m_Device(irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_OPENGL)),
        m_PlayerNode(m_Device)
    {
    }
    void Game::run()
    {
        auto sceneManager = m_Device->getSceneManager();
        m_Device->getCursorControl()->setVisible(false);

        m_Device->setEventReceiver(&m_EventManager);

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
        auto nodePosition = m_PlayerNode.PlayerNode->getPosition();
        const irr::f32 MOVEMENT_SPEED = 0.5f;

        if(m_EventManager.IsKeyDown(irr::KEY_KEY_W))
        {
            nodePosition.Z += MOVEMENT_SPEED;
        }
        else if(m_EventManager.IsKeyDown(irr::KEY_KEY_S))
        {
            nodePosition.Z -= MOVEMENT_SPEED;
        }
        if(m_EventManager.IsKeyDown(irr::KEY_KEY_A))
        {
            nodePosition.X -= MOVEMENT_SPEED;
        }
        else if(m_EventManager.IsKeyDown(irr::KEY_KEY_D))
        {
            nodePosition.X += MOVEMENT_SPEED;
        }

        m_PlayerNode.PlayerNode->setPosition(nodePosition);
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
        while (m_FixedUpdateTimer >= FixedUpdateInterval)
        {
            m_FixedUpdateTimer -= FixedUpdateInterval;
            fixedUpdate();
        }
    }

    void Game::fixedUpdate()
    {
    }

    void Game::render()
    {
    m_Device->getVideoDriver()->beginScene(true, true, irr::video::SColor(255, 100, 101, 140));
    m_Device->getSceneManager()->drawAll();
    m_Device->getGUIEnvironment()->drawAll();
    m_Device->getVideoDriver()->endScene();
    }
}
