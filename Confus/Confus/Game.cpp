#include <Irrlicht/irrlicht.h>

#include "Game.h"
#include "OpenALSource.h"
#include "OpenALListener.h"

const double Game::FixedUpdateInterval = 0.02;
const double Game::MaxFixedUpdateInterval = 0.1;

void Game::run()
{ 
    //Create Game Device
	m_Device = irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_OPENGL);
	m_Device->setWindowCaption(L"Confus");

	//Create Sound Listener
	m_Listener = new OpenALListener();

	//Test Sound
	int position = 5;
	OpenALSource* source = new OpenALSource();
	source->init("Footsteps.wav");
	source->setPlaySpeed(5);
	source->setPosition(position, 0, 0);
	source->setLoop(true);
	source->play();

	//Game Loop
    while(m_Device->run())
    {
        handleInput();
        update();
        processFixedUpdates();
        render();

		//Test sound
		if (source->isPlaying() == false) {
			source->setPosition(--position, 0, 0);
			source->play();
		}
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
