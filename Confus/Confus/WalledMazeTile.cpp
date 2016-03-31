#include "WalledMazeTile.h"

namespace Confus
{

	WalledMazeTile::WalledMazeTile(irr::IrrlichtDevice* a_Device, irr::core::vector3df a_RealPosition, irr::core::vector3df a_HiddenPosition)
		:m_Wall(a_Device, a_RealPosition, a_HiddenPosition)
	{
	}

	void WalledMazeTile::fixedUpdate()
	{
		m_Wall.fixedUpdate();
	}

	MoveableWall * WalledMazeTile::getWall()
	{
		return &m_Wall;
	}


	WalledMazeTile::~WalledMazeTile()
	{
	}
}
