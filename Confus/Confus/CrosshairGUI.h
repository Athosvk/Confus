#pragma once
#include <Irrlicht\irrlicht.h>

#include "IUIElement.h"

namespace Confus
{
    class CrosshairGUI : public IUIElement
    {
    private:
        irr::gui::IGUIImage* m_CrosshairImage;
        irr::video::ITexture* m_CrosshairTexture;
        irr::core::dimension2du m_ImageMaxSize;

    public:
        CrosshairGUI(irr::IrrlichtDevice* a_Device, irr::core::dimension2du a_ImageMaxSize, irr::video::ITexture* a_CrosshairTexture, irr::core::vector2df a_RelativePosition);
        virtual void update() override;
        ~CrosshairGUI();
    };
}

