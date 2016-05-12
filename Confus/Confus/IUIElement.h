#pragma once
#include <Irrlicht\irrlicht.h>

class IUIElement
{
public:
	virtual void update() = 0;
	virtual ~IUIElement() = default;

	static irr::core::dimension2du calculateOffset(irr::core::dimension2du a_ImageMaxSize, irr::video::ITexture * a_Texture)
	{
		irr::core::dimension2du offset;
		irr::core::dimension2du textureSize = a_Texture->getSize();
		if (a_ImageMaxSize.Width <= textureSize.Width)
		{
			offset.Width = a_ImageMaxSize.Width;
		}
		else
		{
			offset.Width = textureSize.Width;
		}

		if (a_ImageMaxSize.Height <= textureSize.Height)
		{
			offset.Height = a_ImageMaxSize.Height;
		}
		else
		{
			offset.Height = textureSize.Height;
		}
		return offset;
	}
	static irr::gui::IGUIImage* addImage(irr::gui::IGUIEnvironment* a_GUIEnvironment, irr::core::position2d<int> a_ScreenPixelPosition, irr::video::ITexture* a_Texture, irr::video::SColor a_Color, irr::core::dimension2du a_ImageMaxSize, bool a_ScaleImage = true, bool a_Visible = true)
	{
		auto image = a_GUIEnvironment->addImage(a_Texture, a_ScreenPixelPosition);
		image->setColor(a_Color);
		image->setScaleImage(a_ScaleImage);
		image->setMaxSize(a_ImageMaxSize);
		image->setVisible(a_Visible);
		return image;
	}
};

