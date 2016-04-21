#include "Game.h"
#include <Irrlicht\irrlicht.h>

int main()
{
	irr::IrrlichtDevice *nullDevice = irr::createDevice(irr::video::EDT_NULL);

	irr::core::dimension2d<irr::u32> desktopResolution = nullDevice->getVideoModeList()->getDesktopResolution();

	nullDevice->drop();
	irr::core::dimension2d<irr::u32> fixedResolution = irr::core::dimension2d<irr::u32>(1366, 768);
    Confus::Game game(fixedResolution);
    game.run();

    return 0;
}