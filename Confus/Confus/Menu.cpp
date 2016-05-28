#include <Irrlicht/irrlicht.h>
#include <iostream>

#define DEBUG_CONSOLE

#include "Menu.h"
#include "../ConfusShared/Debug.h"
#include "Game.h"
#include "../ConfusShared/EventManager.h"

namespace Confus
{
	Menu::Menu(irr::IrrlichtDevice* a_Device, ConfusShared::EventManager* a_EventManager)
		: BaseGame(a_Device, a_EventManager)
	{
	}

    void Menu::start()
    {
        m_Device->setWindowCaption(L"Menu");
        m_Text = m_Device->getGUIEnvironment()->addStaticText(L"Press SPACE to start the game, BACKSPACE to exit.", { 10, 500, 10 + 250, 500 + 80 });
    }

    void Menu::update()
    {
        if(m_EventManager->IsKeyDown(irr::KEY_BACK))
        {
            m_ShouldRun = false;
        }
        else if(m_EventManager->IsKeyDown(irr::KEY_SPACE))
        {
            m_Text->setText(L"Enter the ip of the host and the port of the host in the console now!");
            render();

            m_Text->setText(L"Press ESCAPE to go back to the main menu");
            runGame();
            m_Device->getGUIEnvironment()->clear();
            m_Text = m_Device->getGUIEnvironment()->addStaticText(L"Press SPACE to start the game, BACKSPACE to exit.", { 10, 500, 10 + 250, 500 + 80 });
        }
    }

    void Menu::fixedUpdate()
    {
    }

    void Menu::end()
    {
        if(m_Text != nullptr)
        {
            m_Text->remove();
        }
    }

    void Menu::runGame()
    {
        Game game(m_Device, m_EventManager);
        game.run();
        m_Device->setWindowCaption(L"Menu");
    }
}