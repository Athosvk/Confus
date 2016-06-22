#include "NewMazeTile.h"


namespace ConfusShared
{

	NewMazeTile::NewMazeTile()
		:Raised(true)
	{
	}

	bool NewMazeTile::IsAvailable()
	{
		if (!Raised || Wall)
			return false;
		else
			return true;
	}

	NewMazeTile::~NewMazeTile()
	{
	}
}