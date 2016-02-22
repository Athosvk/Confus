#include <Irrlicht/irrlicht.h>

#include "Game.h"
#include "OpenALAudio.h"
#include <sstream>

const double Game::FixedUpdateInterval = 0.02;
const double Game::MaxFixedUpdateInterval = 0.1;

void Game::run()
{ 
    m_Device = irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_OPENGL);

	//Test
	OpenALAudio sound;
	std::ostringstream oss;
	oss << "Played Sound: " << sound.PlayASound();
	

	irr::ILogger* logger = m_Device->getLogger();
	logger->log(oss.str().c_str());
	logger->log("Test");

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
