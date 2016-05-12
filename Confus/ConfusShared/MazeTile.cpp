#include "MazeTile.h"


namespace Confus
{

	MazeTile::MazeTile()
		:Raised(true)
	{
	}

	void MazeTile::fixedUpdate()
	{
	}

	MoveableWall* MazeTile::getWall()
	{
		return nullptr;
	}


	MazeTile::~MazeTile()
	{
	}
}