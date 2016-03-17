#pragma once
#include <string>
#include "MoveableWall.h"
#include <memory>
namespace Confus
{
	class MazeTile
	{
	public:
		bool Raised = true;
		int XPos = 0, YPos = 0;
		MoveableWall Wall;
	public:
		MazeTile(irr::IrrlichtDevice * a_Device, irr::core::vector3df a_RealPosition, irr::core::vector3df a_HiddenPosition);
		~MazeTile();
	};
}

