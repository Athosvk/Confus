#include <Irrlicht/irrlicht.h>

#include "Game.h"

void Game::run()
{ 
    m_Device = irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_OPENGL);
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
}

void Game::fixedUpdate()
{
}

void Game::render()
{
}
