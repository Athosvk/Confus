#include "FlagStatus.h"
#include "GUI.h"
#include "Game.h"


namespace Confus
{
	FlagStatus::FlagStatus(irr::gui::IGUIEnvironment* a_GUIEnvironment, irr::video::IVideoDriver* a_Driver)
		:m_GUIEnvironment(a_GUIEnvironment), m_Driver(a_Driver)
	{
		m_ExclamationMarkTexture = m_Driver->getTexture("Media/Textures/ExclamationMark.png");
		m_FlagTexture = m_Driver->getTexture("Media/Textures/FlagUIUmage.png");
		m_RedExclamationMark = m_GUIEnvironment->addImage(m_FlagTexture, GUI::CalculatePixelPosition(irr::core::vector2df(0.45f, 0.1f)));
		m_RedExclamationMark->setColor(irr::video::SColor(255, 255, 0, 0));
		m_RedFlag = m_GUIEnvironment->addImage(m_FlagTexture, GUI::CalculatePixelPosition(irr::core::vector2df(0.45f, 0.1f)) + irr::core::position2d<int>(m_ExclamationMarkTexture->getSize().Width, m_ExclamationMarkTexture->getSize().Height));
		m_RedFlag->setColor(irr::video::SColor(255, 255, 0, 0));
		m_BlueExclamationMark = m_GUIEnvironment->addImage(m_FlagTexture, GUI::CalculatePixelPosition(irr::core::vector2df(0.55f, 0.1f)));
		m_BlueExclamationMark->setColor(irr::video::SColor(255, 255, 0, 0));
		m_BlueFlag = m_GUIEnvironment->addImage(m_FlagTexture, GUI::CalculatePixelPosition(irr::core::vector2df(0.55f, 0.1f)) + irr::core::position2d<int>(m_ExclamationMarkTexture->getSize().Width, m_ExclamationMarkTexture->getSize().Height));
		m_BlueFlag->setColor(irr::video::SColor(255, 0, 0, 255));
	}


	FlagStatus::~FlagStatus()
	{
	}
}
