#include "Game.h"
#include <Irrlicht\irrlicht.h>

int main()
{
	irr::IrrlichtDevice *nulldevice = irr::createDevice(irr::video::EDT_NULL);

	irr::core::dimension2d<irr::u32> deskres = nulldevice->getVideoModeList()->getDesktopResolution();

	nulldevice->drop();
    Confus::Game game(deskres);
    game.run();

    return 0;
}