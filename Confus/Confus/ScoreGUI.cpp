#include "ScoreGUI.h"


ScoreGUI::ScoreGUI(irr::IrrlichtDevice * a_Device, Flag * a_Flag, irr::core::dimension2du a_ImageMaxSize, irr::video::ITexture * a_FlagTexture, irr::video::ITexture * a_ExclamationMarkTexuture, irr::core::vector2df a_RelativePosition)
{
}

void ScoreGUI::update()
{
}

irr::core::dimension2du ScoreGUI::calculateOffset(irr::video::ITexture * a_Texture)
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

ScoreGUI::~ScoreGUI()
{
}
