#include <Irrlicht/irrlicht.h>
#include <sstream>

#include "Game.h"
#include "OpenALAudio.h"
#include "Player.h"

namespace Confus
{
    const double Game::FixedUpdateInterval = 0.02;
    const double Game::MaxFixedUpdateInterval = 0.1;

void Game::run()
{ 
    m_Device = irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_OPENGL);
    m_Device->getCursorControl()->setVisible(false);

    auto player = Player(m_Device);

	OpenALAudio sound;
	std::ostringstream oss;
	oss << "Played Sound: " << sound.PlayASound();

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
    }

    void Game::render()
    {
    m_Device->getVideoDriver()->beginScene(true, true, irr::video::SColor(255, 100, 101, 140));
    m_Device->getSceneManager()->drawAll();
    m_Device->getGUIEnvironment()->drawAll();
    m_Device->getVideoDriver()->endScene();
    }
}
