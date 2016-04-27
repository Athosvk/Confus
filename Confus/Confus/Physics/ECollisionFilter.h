#pragma once
#include "../EnumUtility.h"

namespace Confus
{
	namespace Physics
	{
		enum class ECollisionFilter : short
		{
			None = 0,
			MoveableWall = 1,
			Player = 2,
			LevelStatic = 4,
			Other = 8,
			All = None | MoveableWall | Player | LevelStatic | Other
		};
		EnumFlagOperators(ECollisionFilter);
	}
}
