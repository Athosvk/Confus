#pragma once
#include <Irrlicht\irrlicht.h>

using namespace irr;

class Input : public IEventReceiver
{
private:
    Input();
public:
    Input(irr::IrrlichtDevice* a_Device);
    virtual bool OnEvent(const irr::SEvent& event);
    irr::IrrlichtDevice* activeDevice;
    ~Input();
};

