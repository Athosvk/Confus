#include "GUI.h"

#include "CrosshairGUI.h"

namespace Confus
{
    CrosshairGUI::CrosshairGUI(irr::IrrlichtDevice* a_Device,irr::core::dimension2du a_ImageSize, irr::video::ITexture * a_CrosshairTexture, irr::core::vector2df a_RelativePosition)
    {
        irr::core::dimension2du offset = calculateOffset(a_ImageSize, a_CrosshairTexture);

        irr::gui::IGUIEnvironment* guiEnvironment = a_Device->getGUIEnvironment();
        irr::video::IVideoDriver* driver = a_Device->getVideoDriver();


        m_CrosshairImage = addImage(guiEnvironment, GUI::calculatePixelPosition(a_RelativePosition, a_Device->getVideoDriver()->getScreenSize()) - irr::core::dimension2di(offset.Width / 2, 0),
            a_CrosshairTexture, irr::video::SColor(255, 255, 255, 255), a_ImageSize);
    }

    void CrosshairGUI::update()
    {

    }


    CrosshairGUI::~CrosshairGUI()
    {
    }
}
