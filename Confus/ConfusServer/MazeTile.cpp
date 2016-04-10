#include "MazeTile.h"


namespace ConfusServer
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