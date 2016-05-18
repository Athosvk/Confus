#include "FlagGUI.h"
#include "GUI.h"

namespace Confus
{
	FlagGUI::FlagGUI(irr::IrrlichtDevice* a_Device, ConfusShared::Flag* a_Flag, irr::core::dimension2du a_ImageMaxSize, irr::video::ITexture * a_FlagTexture, irr::video::ITexture * a_ExclamationMarkTexuture, irr::core::vector2df a_RelativePosition, bool a_LeftSide)
		:m_Flag(a_Flag), m_ImageMaxSize(a_ImageMaxSize)
	{
		irr::core::dimension2du offset = calculateOffset(m_ImageMaxSize, a_FlagTexture);
		offset = calculateOffset(m_ImageMaxSize, a_ExclamationMarkTexuture);
		auto guiEnvironment = a_Device->getGUIEnvironment();
		auto videoDriver = a_Device->getVideoDriver();
		irr::video::SColor flagColor = a_Flag->getColor();

		m_ExclamationMarkImage = addImage(guiEnvironment, GUI::calculatePixelPosition(a_RelativePosition, a_Device->getVideoDriver()->getScreenSize()) - irr::core::dimension2di(offset.Width / 2, 0),
			a_ExclamationMarkTexuture, flagColor, m_ImageMaxSize);

		irr::core::vector2d<irr::s32> rect;

		if (a_LeftSide)
		{
			rect = m_ExclamationMarkImage->getRelativePosition().UpperLeftCorner - irr::core::dimension2di(offset.Width, 0);
		}
		else
		{
			rect = m_ExclamationMarkImage->getRelativePosition().UpperLeftCorner + irr::core::dimension2di(offset.Width, 0);
		}

		m_FlagImage = addImage(guiEnvironment, rect, a_FlagTexture, flagColor, m_ImageMaxSize);
	}

	void FlagGUI::changeFlagState()
	{
		if ((m_Flag->getFlagStatus()) != ConfusShared::EFlagEnum::FlagBase)
		{
			m_ExclamationMarkImage->setVisible(true);
		}
		else
		{
			m_ExclamationMarkImage->setVisible(false);
		}
	}

	void FlagGUI::update()
	{
		changeFlagState();
	}

	FlagGUI::~FlagGUI()
	{
	}
}
