#include "Input.h"
#include "Game.h"
#include <iostream>

Input::Input(irr::IrrlichtDevice* a_Device)
{
    activeDevice = a_Device;
}


Input::~Input()
{
}

bool Input::OnEvent(const irr::SEvent& event)
{
    // Quit application on escape button press.
    if(event.EventType == irr::EET_KEY_INPUT_EVENT)
    {
        if(event.KeyInput.Key == irr::KEY_ESCAPE)
        {
            activeDevice->closeDevice();
        }
    }
    return false;
}

