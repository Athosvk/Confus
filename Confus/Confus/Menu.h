#pragma once
#include "../ConfusShared/BaseGame.h"

namespace Confus
{
    class Menu : public ConfusShared::BaseGame
    {
    private:
        irr::gui::IGUIStaticText* m_Text;
    private:
        virtual void start() override;
        virtual void update() override;
        virtual void fixedUpdate() override;
        virtual void end() override;
        void runGame();
    public:
		Menu(irr::IrrlichtDevice* a_Device, ConfusShared::EventManager* a_EventManager);
    };
}