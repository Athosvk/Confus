#pragma once
#include "BaseGame.h"
#include "../Common/TeamIdentifier.h"

namespace Confus
{
    class WinScreen : public BaseGame
    {
    private:
        irr::gui::IGUIStaticText* m_WinningText;
        ETeamIdentifier m_GameWinner;
        static const double BreakTime;
        double m_BreakTimer = 0.0;

    private:
        virtual void start() override;
        virtual void update() override;
        virtual void fixedUpdate() override;
        virtual void end() override;
        virtual void render() override;
    public:
        bool ShouldRestart = false;
        WinScreen(irr::IrrlichtDevice* a_Device, ETeamIdentifier a_TeamIdentifier, EventManager* a_EventManager) : 
            BaseGame(a_Device, a_EventManager),
            m_GameWinner(a_TeamIdentifier) {}
    };
}