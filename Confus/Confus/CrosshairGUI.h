#pragma once
#include <Irrlicht\irrlicht.h>

#include "IUIElement.h"

namespace Confus
{
    /// <summary> 
    /// A class that constructs the crosshair GUI element. 
    ///       ----------
    ///      |IUIElement|
    ///       ----------
    ///           ^
    ///           |
    ///      ------------
    ///     |CrosshairGUI|
    ///      ------------
    /// </summary>
    class CrosshairGUI : public IUIElement
    {
    private:
        /// <summary> A reference to the crosshair image. </summary>
        irr::gui::IGUIImage* m_CrosshairImage;
    public:
        /// <summary> Crosshair constructor. </summary>
        /// <param name="a_Device">The active Irrlicht Device.</param>
        /// <param name="a_ImageSize">The size of the image on the screen.</param>
        /// <param name="a_CrosshairTexture">The texture to display.</param>
        /// <param name="a_RelativePosition">The position on the screen, being 0 and 1 at the edges of the screen. </param>
        CrosshairGUI(irr::IrrlichtDevice* a_Device, irr::core::dimension2du a_ImageSize, irr::video::ITexture* a_CrosshairTexture, irr::core::vector2df a_RelativePosition);
        virtual void update() override;
        ~CrosshairGUI();
    };
}

