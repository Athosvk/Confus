#include "MazeTile.h"


namespace Confus
{

	MazeTile::MazeTile(irr::IrrlichtDevice* a_Device, irr::core::vector3df a_RealPosition, irr::core::vector3df a_HiddenPosition)
		:Raised(true), Wall(a_Device,a_RealPosition,a_HiddenPosition)
	{
	}


	MazeTile::~MazeTile()
	{
	}
}