#include <Irrlicht/irrlicht.h>

#include "GUI.h"
#include "Game.h"
#include "Audio\AudioManager.h"

namespace Confus
{
    GUI::GUI(irr::IrrlichtDevice* a_Device, Player* a_Player, Audio::AudioManager* a_AudioManager)
        : m_AudioSourceLowHealth(a_AudioManager->createSound("heartbeat.wav"))
	{
		m_Device = a_Device;
		m_GUIEnvironment = a_Device->getGUIEnvironment();
		m_Driver = a_Device->getVideoDriver();
		m_BloodImage = m_Driver->getTexture("Media/Textures/Blood.png");
		m_BloodOverlay = m_GUIEnvironment->addImage(m_BloodImage, irr::core::position2d<int>(0, 0));
		m_PlayerNode = a_Player;
		m_HealthTextBox = m_GUIEnvironment->addStaticText(L"", irr::core::rect<irr::s32>(10, 10, 100, 25), false);
	}

	GUI::~GUI()
	{
	}

	void GUI::update()
	{
		for (auto& uiElement : m_Elements)
		{
			uiElement->update();
		}
		m_HealthString = (L"Health: " + std::to_wstring(m_PlayerNode->PlayerHealth.getHealth()) + L"%").c_str();
		m_HealthTextBox->setText(m_HealthString.c_str());
		drawBloodOverlay();
		lowHealthAudio();
	}

	void GUI::drawBloodOverlay()
	{
		m_BloodOverlay->setVisible(m_PlayerNode->PlayerHealth.getHealth() <= 50);
	}

	void GUI::lowHealthAudio()
	{
		if (m_PlayerNode->PlayerHealth.getHealth() <= 25)
		{
			m_AudioSourceLowHealth.play();
		}
		else
		{
			m_AudioSourceLowHealth.stop();
		}
	}

	irr::core::position2d<int> GUI::calculatePixelPosition(irr::core::vector2df a_Coordinates, irr::core::dimension2d<irr::u32> a_CurrentResolution)
	{
		irr::core::vector2df pixelCoords;
		pixelCoords.X = a_CurrentResolution.Width * a_Coordinates.X;
		pixelCoords.Y = a_CurrentResolution.Height * a_Coordinates.Y;

		return irr::core::position2d<int>(static_cast<int>(pixelCoords.X), static_cast<int>(pixelCoords.Y));
	}
}
