#pragma once
#include <Irrlicht\irrlicht.h>
#include <string>

#include "../ConfusShared/Flag.h"
#include "IUIElement.h"

namespace Confus
{	
	/// <summary>
	/// The GUI element that represents the flagstate of each team's flag
	/// </summary>
	/// <seealso cref="IUIElement" />
	class FlagGUI : public IUIElement
	{
	private:
		/// <summary>The flag to represent</summary>
		ConfusShared::Flag* m_Flag;
		/// <summary>The image to show for the flag</summary>
		irr::gui::IGUIImage* m_FlagImage;
		/// <summary>
		/// The image of the exclamation mark that is used to show that the flag is 
		/// currently not in the team's base
		/// </summary>
		irr::gui::IGUIImage* m_ExclamationMarkImage;
		/// <summary>The texture for the flag image</summary>
		irr::video::ITexture* m_FlagTexture;
		/// <summary>The texture for the exclamation mark image</summary>
		irr::video::ITexture* m_ExclamationMarkTexture;
		/// <summary>
		/// The maximum size of either image, to restrict it from scaling too far up with the 
		/// screen resolution
		/// </summary>
		irr::core::dimension2du m_ImageMaxSize;

	public:
		/// <summary>Initializes a new instance of the <see cref="FlagGUI"/> class.</summary>
		/// <param name="a_Device">The active Irrlicht device</param>
		/// <param name="a_Flag">The flag to represent the state of</param>
		/// <param name="a_ImageMaxSize">The maximum size the images of the FlagGUI can have</param>
		/// <param name="a_FlagTexture">The texture for the flag image</param>
		/// <param name="a_ExclamationMarkTexuture">The texture for the exclamation mark image</param>
		/// <param name="a_RelativePosition">The relative screen position</param>
		/// <param name="a_LeftSide">
		/// if set to <c>true</c>, it will be on the left side of the screen, so 
		/// it can take care of mirroring the image when necessary. Otherwise on the right side of the screen 
		/// </param>
		FlagGUI(irr::IrrlichtDevice* a_Device, ConfusShared::Flag* a_Flag, irr::core::dimension2du a_ImageMaxSize, irr::video::ITexture* a_FlagTexture, irr::video::ITexture* a_ExclamationMarkTexuture, irr::core::vector2df a_RelativePosition, bool a_LeftSide);
				
		/// <summary>Updates the state of the exclamation mark image/flag image based on teh new flag state</summary>
		void changeFlagState();

		/// <summary>Updates this instance.</summary>
		virtual void update() override;

		/// <summary>Finalizes an instance of the <see cref="FlagGUI"/> class.</summary>
		~FlagGUI();
	};
}

