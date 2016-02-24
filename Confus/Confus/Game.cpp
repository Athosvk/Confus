#include <Irrlicht/irrlicht.h>
#include "StaticWall.h"
#include "Input.h"
#include <iostream>

#include "Game.h"

const double Game::FixedUpdateInterval = 0.02;
const double Game::MaxFixedUpdateInterval = 0.1;

void Game::run()
{ 
    m_Device = irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_OPENGL);

    smgr = m_Device->getSceneManager();
    driver = m_Device->getVideoDriver();
    
    StaticWall(m_Device, core::vector3d<float>(0.0f, 0.0f, 0.0f), core::vector3d<float>(0.0f, 90.0f, 90.0f)); 
    Input* inputDevice = new Input(m_Device);

      while(m_Device->run())
    {
        handleInput();
        update();
        processFixedUpdates();
        render();
    }

    m_Device->drop();
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
    int lastFPS = -1;

    if(m_Device->isWindowActive())
    {
        driver->beginScene(true, true, video::SColor(255, 200, 200, 200));
        smgr->drawAll();
        driver->endScene();

        int fps = driver->getFPS();

        if(lastFPS != fps)
        {
            core::stringw str = L"Confus  [";
            str += driver->getName();
            str += "] FPS:";
            str += fps;

            m_Device->setWindowCaption(str.c_str());
            lastFPS = fps;
        }
    }
    else 
    {
        m_Device->yield();
    }
}





