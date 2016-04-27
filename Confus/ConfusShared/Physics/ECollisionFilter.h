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
			Interactable = 8,
			Other = 16,
			All = None | MoveableWall | Player | LevelStatic | Interactable | Other
		};
		EnumFlagOperators(ECollisionFilter);
	}
}
