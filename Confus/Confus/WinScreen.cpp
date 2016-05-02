#include <string>

#include "WinScreen.h"

namespace Confus
{

    const double WinScreen::BreakTime = 10;
    void WinScreen::start()
    {
        m_WinningText = m_Device->getGUIEnvironment()->addStaticText(L"Resetting Game", { 10, 240, 10 + 150, 240 + 80 });
        m_Device->setWindowCaption(L"Win Screen");
    }

    void WinScreen::update()
    {

        

        m_BreakTimer += m_DeltaTime;


        // Show text on GUI
        if(m_GameWinner == ETeamIdentifier::TeamBlue)
        {
            m_WinningText->setText((L"Blue team won! \nPress SPACE to restart now! \nPress ESCAPE to exit.\nExitting in: " + std::to_wstring(BreakTime - m_BreakTimer)).c_str());
        }
        else if(m_GameWinner == ETeamIdentifier::TeamRed)
        {
            m_WinningText->setText((L"Red team won! \nPress SPACE to restart now! \nPress ESCAPE to exit.\nExitting in: " + std::to_wstring(BreakTime - m_BreakTimer)).c_str());
        }

        if(m_EventManager.IsKeyDown(irr::KEY_SPACE))
        {
            m_ShouldRun = false;
            ShouldRestart = true;
        } 
        else if(m_EventManager.IsKeyDown(irr::KEY_ESCAPE) || m_BreakTimer >= BreakTime)
        {
            m_ShouldRun = false;
            ShouldRestart = false;
        }
    }

    void WinScreen::fixedUpdate()
    {
    }

    void WinScreen::end()
    {
        m_WinningText->remove();
    }

    void WinScreen::render()
    {
        m_Device->getVideoDriver()->beginScene(true, true, irr::video::SColor(255, 150, 150, 150));
        m_Device->getGUIEnvironment()->drawAll();
        m_Device->getVideoDriver()->endScene();
    }
}