#include "WalledMazeTile.h"

namespace Confus
{

	WalledMazeTile::WalledMazeTile(irr::IrrlichtDevice* a_Device, irr::core::vector3df a_RealPosition, irr::core::vector3df a_HiddenPosition)
		:Wall(a_Device, a_RealPosition, a_HiddenPosition)
	{
	}

	void WalledMazeTile::fixedUpdate()
	{
		Wall.fixedUpdate();
	}

	MoveableWall * WalledMazeTile::getWall()
	{
		return &Wall;
	}


	WalledMazeTile::~WalledMazeTile()
	{
	}
}
