#include "GUI.h"

#include "CrosshairGUI.h"

namespace Confus
{
    CrosshairGUI::CrosshairGUI(irr::IrrlichtDevice* a_Device,irr::core::dimension2du a_ImageMaxSize, irr::video::ITexture * a_CrosshairTexture, irr::core::vector2df a_RelativePosition)
        : m_ImageMaxSize(a_ImageMaxSize)
    {
        irr::core::dimension2du offset = calculateOffset(m_ImageMaxSize, a_CrosshairTexture);

        irr::gui::IGUIEnvironment* guiEnvironment = a_Device->getGUIEnvironment();
        irr::video::IVideoDriver* driver = a_Device->getVideoDriver();


        m_CrosshairImage = addImage(guiEnvironment, GUI::calculatePixelPosition(a_RelativePosition, a_Device->getVideoDriver()->getScreenSize()) - irr::core::dimension2di(offset.Width / 2, 0),
            a_CrosshairTexture, irr::video::SColor(255, 255, 255, 255), a_ImageMaxSize);
    }

    void CrosshairGUI::update()
    {

    }


    CrosshairGUI::~CrosshairGUI()
    {
    }
}
