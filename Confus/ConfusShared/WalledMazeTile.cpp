#include "WalledMazeTile.h"

namespace ConfusShared
{
	WalledMazeTile::WalledMazeTile(irr::IrrlichtDevice* a_Device, irr::core::vector3df a_RealPosition, irr::core::vector3df a_Scale,
		Physics::PhysicsWorld& a_World)
		:m_Wall(a_Device, a_RealPosition, a_Scale, a_World)
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
