#pragma once
#include <irrlicht/irrlicht.h>
#include <array>

namespace ConfusShared
{
    /// <summary> 
    /// Event Manager
    /// </summary>
    class EventManager : public irr::IEventReceiver
    {
    private:
        /// <summary> An array of all irrlicht keycodes, set to false initially. </summary>
        std::array<bool, irr::KEY_KEY_CODES_COUNT> m_KeyIsDown = {false};
        /// <summary> Struct for the mouse state. </summary>
        struct MouseState
        {
            bool LeftButtonDown;
            bool RightButtonDown;
            MouseState() : LeftButtonDown(false), RightButtonDown(false) { }
        } m_MouseState;
    public:
        /// <summary> Constructor for the Event Manager. </summary>
        EventManager();
        /// <summary> OnEvent is called automatically by the active device. </summary>
        /// <param name="a_Event">The specific event that occurred.</param>
        virtual bool OnEvent(const irr::SEvent& a_Event);
        /// <summary> Function returns a bool depending on the state of the key given. </summary>
        /// <param name="a_KeyCode">The key that should be checked.</param>
        bool IsKeyDown(irr::EKEY_CODE a_KeyCode) const;
        /// <summary> Returns a bool depending on the state of the left mouse button. </summary>
        bool IsLeftMouseDown() const;
        /// <summary> Returns a bool depending on the state of the right mouse button. </summary>
        bool IsRightMouseDown() const;
    };
}