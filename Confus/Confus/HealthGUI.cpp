#include "HealthGUI.h"
#include "GUI.h"
#include "Game.h"
#include "ClientTeamScore.h"

namespace Confus
{
	const int Increment = 4; //Using the magic number 4, because we decrease the health by 25%

	HealthGUI::HealthGUI(irr::IrrlichtDevice* a_Device, Player* a_PlayerNode, irr::core::dimension2du a_ImageMaxSize, irr::video::ITexture* a_HealthTexture, irr::video::ITexture* a_BloodTexture, irr::core::vector2df a_RelativePosition)
		:m_ImageMaxSize(a_ImageMaxSize), m_PlayerNode(a_PlayerNode)
	{
		irr::core::dimension2du offset = calculateOffset(m_ImageMaxSize, a_HealthTexture);
		auto guiEnvironment = a_Device->getGUIEnvironment();
		auto videoDriver = a_Device->getVideoDriver();

		for (int i = 0; i < Increment; i++) 
		{
			m_HealthImages.push_back(addImage(guiEnvironment, GUI::calculatePixelPosition(a_RelativePosition, a_Device->getVideoDriver()->getScreenSize()) + irr::core::dimension2di(offset.Width / 2 + i*(offset.Width + offset.Width / 4), 0),
				a_HealthTexture, irr::video::SColor(255, 255, 255, 255), m_ImageMaxSize));
		}
		
		for (int i = 0; i < Increment; i++)
		{
			m_BloodImages.push_back(addImage(guiEnvironment, irr::core::vector2d<irr::s32>(0, 0),
				a_BloodTexture, irr::video::SColor(255, 255, 255, 255), a_Device->getVideoDriver()->getScreenSize()));
		}
	}

	void HealthGUI::update()
	{
		for (int i = 0; i < Increment; i++)
		{
			m_HealthImages[i]->setVisible(m_PlayerNode->getHealthInstance()->getHealth() >= i * (m_PlayerNode->getHealthInstance()->getMaxHealth() / Increment));
			m_BloodImages[i]->setVisible(m_PlayerNode->getHealthInstance()->getHealth() <= i * (m_PlayerNode->getHealthInstance()->getMaxHealth() / Increment));
		}
	}

	HealthGUI::~HealthGUI()
	{
	}
}
