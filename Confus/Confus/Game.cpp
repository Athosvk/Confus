#include <Irrlicht/irrlicht.h>

#include "Game.h"
#include "OpenAL/OpenALSource.h"
#include "OpenAL/OpenALListener.h"

const double Game::FixedUpdateInterval = 0.02;
const double Game::MaxFixedUpdateInterval = 0.1;

void Game::run()
{ 
    //Create Game Device
	m_Device = irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_OPENGL);
	m_Device->setWindowCaption(L"Confus");

	//Create Sound 
    m_Listener = new Confus::OpenALListener();
    m_Listener->init();

	//Game Loop
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
    while(m_FixedUpdateTimer >= FixedUpdateInterval)
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
}

void Game::shutdown() 
{
    m_Listener->dispose();
}
