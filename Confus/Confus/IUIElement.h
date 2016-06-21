#pragma once
#include <Irrlicht\irrlicht.h>

/// <summary>
/// Represents an UIElement interface that can be added to the GUI, providing helper methods to make construction
/// easier
/// </summary>
class IUIElement
{
public:
	/// <summary>Updates this instance.</summary>
	virtual void update() = 0;
		
	/// <summary>Finalizes an instance of the <see cref="IUIElement"/> class.</summary>
	virtual ~IUIElement() = default;

	/// <summary>Calculates the offset needed to present the given image</summary>
	/// <param name="a_ImageMaxSize">Maximum size of the image</param>
	/// <param name="a_Texture">The texture associated with the image</param>
	/// <returns>The resulting offset</returns>
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
	
	/// <summary>Constructs and adds a new image.</summary>
	/// <param name="a_GUIEnvironment">The GUI environment to add the image to </param>
	/// <param name="a_ScreenPixelPosition">The relative screen position for the image</param>
	/// <param name="a_Texture">The texture to create an image instance for</param>
	/// <param name="a_Color">Color of the texture</param>
	/// <param name="a_ImageMaxSize">Maximum size of the image</param>
	/// <param name="a_ScaleImage">if set to <c>true</c>, it will scale the image</param>
	/// <param name="a_Visible">if set to <c>true</c>, it will start visible</param>
	/// <returns>A handle to the created GUI image</returns>
	static irr::gui::IGUIImage* addImage(irr::gui::IGUIEnvironment* a_GUIEnvironment, irr::core::position2d<int> a_ScreenPixelPosition, 
		irr::video::ITexture* a_Texture, irr::video::SColor a_Color, irr::core::dimension2du a_ImageMaxSize, 
		bool a_ScaleImage = true, bool a_Visible = true)
	{
		auto image = a_GUIEnvironment->addImage(a_Texture, a_ScreenPixelPosition);
		image->setColor(a_Color);
		image->setScaleImage(a_ScaleImage);
		image->setMaxSize(a_ImageMaxSize);
		image->setVisible(a_Visible);
		return image;
	}
};

