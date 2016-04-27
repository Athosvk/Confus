#include "WalledMazeTile.h"

namespace Confus
{

	WalledMazeTile::WalledMazeTile(irr::IrrlichtDevice* a_Device, irr::core::vector3df a_RealPosition)
		:m_Wall(a_Device, a_RealPosition)
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
