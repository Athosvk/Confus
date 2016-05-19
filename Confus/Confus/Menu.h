#pragma once

#include "BaseGame.h"

namespace Confus
{
    class Menu : public BaseGame
    {
    private:
        irr::gui::IGUIStaticText* m_Text;
		irr::gui::IGUIImage* m_Image;
		irr::video::ITexture* m_StartScreen;
		irr::video::ITexture* m_InfoScreen;
		irr::core::rect<irr::s32> m_ScreenRect;
    private:
        virtual void start() override;
        virtual void update() override;
        virtual void fixedUpdate() override;
        virtual void end() override;
        void runGame();
    public:
        Menu(irr::IrrlichtDevice* a_Device, EventManager* a_EventManager) : BaseGame(a_Device, a_EventManager) {}
    };
}