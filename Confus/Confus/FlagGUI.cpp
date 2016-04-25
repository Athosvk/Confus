#include "FlagGUI.h"
#include "GUI.h"

namespace Confus
{
	FlagGUI::FlagGUI(irr::IrrlichtDevice* a_Device, Flag * a_Flag, irr::core::dimension2du a_ImageMaxSize, irr::video::ITexture * a_FlagTexture, irr::video::ITexture * a_ExclamationMarkTexuture, irr::core::vector2df a_RelativePosition, bool a_LeftSide)
		:m_Flag(a_Flag), m_ImageMaxSize(a_ImageMaxSize)
	{
		irr::core::dimension2du offset = calculateOffset(a_FlagTexture);
		offset = calculateOffset(a_ExclamationMarkTexuture);
		auto guiEnvironment = a_Device->getGUIEnvironment();
		auto videoDriver = a_Device->getVideoDriver();
		irr::video::SColor flagColor = a_Flag->getColor();

		m_ExclamationMarkImage = guiEnvironment->addImage(a_ExclamationMarkTexuture, GUI::calculatePixelPosition(a_RelativePosition, a_Device->getVideoDriver()->getScreenSize()) - irr::core::dimension2di(offset.Width/2,0));
		m_ExclamationMarkImage->setColor(flagColor);
		m_ExclamationMarkImage->setScaleImage(true);
		m_ExclamationMarkImage->setMaxSize(m_ImageMaxSize);
		m_ExclamationMarkImage->setVisible(true);

		irr::core::vector2d<irr::s32> rect;

		if (a_LeftSide)
		{
			rect = m_ExclamationMarkImage->getRelativePosition().UpperLeftCorner - irr::core::dimension2di(offset.Width, 0);
		}
		else
		{
			rect = m_ExclamationMarkImage->getRelativePosition().UpperLeftCorner + irr::core::dimension2di(offset.Width, 0);
		}
		m_FlagImage = guiEnvironment->addImage(a_FlagTexture, rect);
		m_FlagImage->setColor(flagColor);
		m_FlagImage->setScaleImage(true);
		m_FlagImage->setMaxSize(m_ImageMaxSize);
	}

	void FlagGUI::changeFlagState()
	{
		if (*(m_Flag->getFlagStatus()) != EFlagEnum::FlagBase)
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

	irr::core::dimension2du FlagGUI::calculateOffset(irr::video::ITexture * a_Texture)
	{
		irr::core::dimension2du offset;
		irr::core::dimension2du textureSize = a_Texture->getSize();
		if (m_ImageMaxSize.Width <= textureSize.Width)
		{
			offset.Width = m_ImageMaxSize.Width;
		}
		else
		{
			offset.Width = textureSize.Width;
		}

		if (m_ImageMaxSize.Height <= textureSize.Height)
		{
			offset.Height = m_ImageMaxSize.Height;
		}
		else
		{
			offset.Height = textureSize.Height;
		}
		return offset;
	}
	FlagGUI::~FlagGUI()
	{
	}
}
