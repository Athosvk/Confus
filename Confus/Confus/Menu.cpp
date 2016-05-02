#include <Irrlicht/irrlicht.h>
#include <iostream>

#define DEBUG_CONSOLE

#include "Menu.h"
#include "../Common/Debug.h"
#include "Game.h"

namespace Confus
{
    void Menu::start()
    {
        m_Device->setWindowCaption(L"Menu");
        m_Text = m_Device->getGUIEnvironment()->addStaticText(L"Press SPACE to start the game, ESCAPE to exit.", { 10, 240, 10 + 250, 240 + 80 });
    }

    void Menu::update()
    {
        if(m_EventManager.IsKeyDown(irr::KEY_ESCAPE))
        {
            m_ShouldRun = false;
        }
        else if(m_EventManager.IsKeyDown(irr::KEY_SPACE))
        {
            m_Text->setText(L"Enter the ip of the host and the port of the host in the console now!");
            render();
            m_Text->remove();
            runGame();
            m_Device->getSceneManager()->clear();
            m_Text = m_Device->getGUIEnvironment()->addStaticText(L"Press space to start the game, Escape to exit.", { 10, 240, 10 + 250, 240 + 80 });
        }
    }

    void Menu::fixedUpdate()
    {
    }

    void Menu::end()
    {
        std::cout << "Goodbye :)!" << std::endl;
        if(m_Text != nullptr)
        {
            m_Text->remove();
        }
    }
    void Menu::runGame()
    {
        Game game(m_Device);
        game.run();
        m_Device->setWindowCaption(L"Menu");
        m_Device->setEventReceiver(&m_EventManager);
    }
}