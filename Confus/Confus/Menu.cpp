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
		: BaseGame(a_Device, a_EventManager),
		m_StartScreen(m_Device->getVideoDriver()->getTexture(("Media/Textures/Menu.png"))),
		m_InfoScreen(m_Device->getVideoDriver()->getTexture(("Media/Textures/MenuInfo.png")))
	{
	}

    void Menu::start()
	{
		m_Device->setWindowCaption(L"Menu");
		auto screenSize = m_Device->getVideoDriver()->getScreenSize();
		m_ScreenRect = irr::core::rect<irr::s32>(0, 0, screenSize.Width, screenSize.Height);
		m_Image = m_Device->getGUIEnvironment()->addImage(m_ScreenRect);
		m_Image->setImage(m_StartScreen);
    }

    void Menu::update()
    {
        if(m_EventManager->IsKeyDown(irr::KEY_BACK))
        {
            m_ShouldRun = false;
        }
        else if(m_EventManager->IsKeyDown(irr::KEY_SPACE))
        {
			m_Image->setImage(m_InfoScreen);
            render();
			m_Image->setVisible(false);
			auto text = m_Device->getGUIEnvironment()->addStaticText(L"Press ESCAPE to go back to the main menu", { 10, 500, 10 + 250, 500 + 80 });
			runGame();
            m_Device->getGUIEnvironment()->clear();
        }
    }

    void Menu::fixedUpdate()
    {
    }

    void Menu::end()
    {
    }

    void Menu::runGame()
    {
        Game game(m_Device, m_EventManager);
        game.run();
        m_Device->setWindowCaption(L"Menu");
    }
}