#include <Irrlicht/irrlicht.h>

#include "GUI.h"

namespace Confus
{
	GUI::GUI(irr::IrrlichtDevice* a_Device, Player* a_Player)
	{
		m_GUIEnvironment = a_Device->getGUIEnvironment();

		m_Driver = a_Device->getVideoDriver();
		m_BloodImage = m_Driver->getTexture("Media/Textures/Blood.png");
		m_BloodOverlay = m_GUIEnvironment->addImage(m_BloodImage, irr::core::position2d<int>(0, 0));
		m_BloodOverlay->setVisible(false);
		m_PlayerNode = a_Player;
		m_HealthTextBox = m_GUIEnvironment->addStaticText(L"", irr::core::rect<irr::s32>(10, 10, 100, 25), false);
		m_AudioSourceLowHealth = new OpenALSource("heartbeat.wav");
	}

	GUI::~GUI()
	{
		delete(m_AudioSourceLowHealth);
	}

	void GUI::update()
	{
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
			m_AudioSourceLowHealth->play();
		}
		else
		{
			m_AudioSourceLowHealth->stop();
		}
	}
}
