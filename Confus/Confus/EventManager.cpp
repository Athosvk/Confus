#include <irrlicht/irrlicht.h>

#include "EventManager.h"

namespace Confus
{
    EventManager::EventManager()
    {
    }

    bool EventManager::OnEvent(const irr::SEvent & a_Event)
    {
        if(a_Event.EventType == irr::EET_MOUSE_INPUT_EVENT)
        {
            switch(a_Event.MouseInput.Event)
            {
            case irr::EMIE_LMOUSE_PRESSED_DOWN:
                m_MouseState.LeftButtonDown = true;
                break;
            case irr::EMIE_LMOUSE_LEFT_UP:
                m_MouseState.LeftButtonDown = false;
                break;
            case irr::EMIE_RMOUSE_PRESSED_DOWN:
                m_MouseState.RightButtonDown = true;
                break;
            case irr::EMIE_RMOUSE_LEFT_UP:
                m_MouseState.RightButtonDown = false;
                break;
            default:
                break;
            }
        }

        if(a_Event.EventType == irr::EET_KEY_INPUT_EVENT)
        {
            m_KeyIsDown[a_Event.KeyInput.Key] = a_Event.KeyInput.PressedDown;
        }

        return false;
    }

    bool EventManager::IsKeyDown(irr::EKEY_CODE a_KeyCode) const
    {
        return m_KeyIsDown[a_KeyCode];
    }

    bool EventManager::IsLeftMouseDown() const
    {
        return m_MouseState.LeftButtonDown;
    }

    bool EventManager::IsRightMouseDown() const
    {
        return m_MouseState.RightButtonDown;
    }
}