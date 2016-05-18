#include "MazeTile.h"

namespace ConfusShared
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