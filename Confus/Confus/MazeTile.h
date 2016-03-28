#pragma once
#include <string>
#include <memory>

#include "MoveableWall.h"

namespace Confus
{
	class MazeTile
	{
	public:
		bool Raised = true;
		int XPos = 0, YPos = 0;
	public:
		MazeTile();
		virtual void fixedUpdate();
		virtual MoveableWall* getWall();
		~MazeTile();
	};
}

