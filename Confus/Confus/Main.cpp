#include <Irrlicht\irrlicht.h>

#include "Menu.h"
#include "../ConfusShared/EventManager.h"

int main()
{
	irr::core::dimension2d<irr::u32> fixedResolution = irr::core::dimension2d<irr::u32>(1366, 768);
    auto device = irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_OPENGL, fixedResolution);

    ConfusShared::EventManager eventManager;
	device->setEventReceiver(&eventManager);

    Confus::Menu menu(device, &eventManager);
    menu.run();
    return 0;
}
