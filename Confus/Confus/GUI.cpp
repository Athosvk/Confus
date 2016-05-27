#include <Irrlicht/irrlicht.h>

#include "GUI.h"
#include "Game.h"
#include "Audio/AudioManager.h"

namespace Confus
{
    GUI::GUI(irr::IrrlichtDevice* a_Device, ConfusShared::Player* a_Player, Audio::AudioManager* a_AudioManager)
        : m_AudioSourceLowHealth(a_AudioManager->createSound("heartbeat.wav"))
	{
		m_Device = a_Device;
		m_GUIEnvironment = a_Device->getGUIEnvironment();
		m_Driver = a_Device->getVideoDriver();
		m_PlayerNode = a_Player;
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
		lowHealthAudio();
	}

	void GUI::lowHealthAudio()
	{
		if (m_PlayerNode->getHealthInstance()->getHealth() <= 25 && !m_AudioSourceLowHealth.isPlaying())
		{
			m_AudioSourceLowHealth.play();
		}
		else if(m_AudioSourceLowHealth.isPlaying())
		{
            if(m_AudioSourceLowHealth.isPlaying())
            {
                m_AudioSourceLowHealth.stop();
            }
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
