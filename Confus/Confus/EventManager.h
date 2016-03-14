#pragma once
#include <irrlicht/irrlicht.h>
#include <array>

namespace Confus
{
    class EventManager : public irr::IEventReceiver
    {
    private:
        std::array<bool, irr::KEY_KEY_CODES_COUNT> m_KeyIsDown = {false};
    public:
        EventManager();
        virtual bool OnEvent(const irr::SEvent& a_Event);
        bool IsKeyDown(irr::EKEY_CODE a_KeyCode) const;
        bool IsLeftMouseDown() const;
        bool IsRightMouseDown() const;
    };
}