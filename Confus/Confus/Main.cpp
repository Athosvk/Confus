#include <Irrlicht\irrlicht.h>

#include "Game.h"
#include "Menu.h"
#include "EventManager.h"

int main()
{
	//irr::IrrlichtDevice *nullDevice = irr::createDevice(irr::video::EDT_NULL);

	//irr::core::dimension2d<irr::u32> desktopResolution = nullDevice->getVideoModeList()->getDesktopResolution();

	//nullDevice->drop();

    //Confus::Game game(fixedResolution);

    irr::core::dimension2d<irr::u32> fixedResolution = irr::core::dimension2d<irr::u32>(1366, 768);
    auto device = irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_OPENGL, fixedResolution);

    Confus::EventManager eventManager;
    Confus::Menu menu(device, &eventManager);
    menu.run();
    device->setEventReceiver(0);

    return 0;
}
