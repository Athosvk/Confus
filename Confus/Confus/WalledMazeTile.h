#pragma once
#include "MazeTile.h"

namespace Confus
{
	class WalledMazeTile : public MazeTile
	{
	public:
		MoveableWall Wall;
	public:
		WalledMazeTile(irr::IrrlichtDevice * a_Device, irr::core::vector3df a_RealPosition, irr::core::vector3df a_HiddenPosition);
		void fixedUpdate();
		MoveableWall* getWall();
		~WalledMazeTile();
	};
}

