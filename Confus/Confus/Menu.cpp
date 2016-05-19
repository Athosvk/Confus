#include <Irrlicht/irrlicht.h>
#include <iostream>

#define DEBUG_CONSOLE

#include "Menu.h"
#include "../ConfusShared/Debug.h"
#include "Game.h"

namespace Confus
{
    void Menu::start()
    {
        m_Device->setWindowCaption(L"Menu");

		auto driver = m_Device->getVideoDriver();
		auto screenSize = driver->getScreenSize();
		m_ScreenRect = irr::core::rect<irr::s32>( 0, 0, screenSize.Width, screenSize.Height);
		m_StartScreen = driver->getTexture("Media/Textures/Menu.png");
		m_InfoScreen = driver->getTexture("Media/Textures/MenuInfo.png");
		m_Image = m_Device->getGUIEnvironment()->addImage(m_ScreenRect);
		m_Image->setImage(m_StartScreen);

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
			m_Image->setImage(m_InfoScreen);
            m_Text->setText(L"Enter the ip of the host and the port of the host in the console now!");
            render();
			m_Image->setVisible(false);
            m_Text->setText(L"Press ESCAPE to go back to the main menu");
            runGame();
            m_Device->getSceneManager()->clear();
            m_Device->getGUIEnvironment()->clear();

			m_Image = m_Device->getGUIEnvironment()->addImage(m_ScreenRect);
			m_Image->setImage(m_StartScreen);
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
		if (m_Image != nullptr)
		{
			m_Image->remove();
		}
		delete(m_StartScreen);
		delete(m_InfoScreen);
    }
    void Menu::runGame()
    {
        Game game(m_Device, m_EventManager);
        game.run();
        m_Device->setWindowCaption(L"Menu");
    }
}