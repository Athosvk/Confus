#include <irrlicht/irrlicht.h>

#include "EventManager.h"

namespace Confus
{
    EventManager::EventManager()
    {
    }

    bool EventManager::OnEvent(const irr::SEvent & a_Event)
    {
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
}