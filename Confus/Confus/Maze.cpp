#include "Maze.h"
#include "WalledMazeTile.h"

namespace Confus
{

	Maze::Maze(irr::IrrlichtDevice* a_Device, irr::core::vector3df a_StartPosition, bool a_NeedRender)
		:MazeSizeX(123), MazeSizeY(80), Identifier(0)
	{
		IrrDevice = a_Device;
		resetMaze(irr::core::vector2df(30, -7), a_NeedRender);
	}

	void Maze::resetMaze(irr::core::vector2df a_Offset, bool a_NeedRender)
	{
		MazeTiles.clear();
		for (int x = 0; x < MazeSizeX; x++)
		{
			std::vector<std::shared_ptr<MazeTile>> tempVector;
			MazeTiles.push_back(tempVector);
			for (int y = 0; y < MazeSizeY - 1; y++)
			{
				if (a_NeedRender)
				{
					std::shared_ptr<WalledMazeTile> mazeTile = std::make_shared<WalledMazeTile>(IrrDevice, irr::core::vector3df(static_cast<float>(-x + a_Offset.X), 0.5f, static_cast<float>(-y + a_Offset.Y)),
															 irr::core::vector3df(static_cast<float>(-x + a_Offset.X), 0.5f, static_cast<float>(-y + a_Offset.Y)));
					const irr::scene::IAnimatedMeshSceneNode* wallMeshNode = mazeTile->Wall.getMeshNode();
					irr::core::vector3df boundingBox = wallMeshNode->getBoundingBox().getExtent();
					mazeTile->Wall.HiddenPosition = irr::core::vector3df(mazeTile->Wall.HiddenPosition.X, -boundingBox.Y * wallMeshNode->getScale().Y, mazeTile->Wall.HiddenPosition.Z);
					mazeTile->Wall.TransitionSpeed = 0.5f;
					MazeTiles[x].push_back(mazeTile);
				}
				else
				{
					std::shared_ptr<MazeTile> mazeTile = std::make_shared<MazeTile>();
					MazeTiles[x].push_back(mazeTile);
				}
				MazeTiles[x][y]->XPos = x;
				MazeTiles[x][y]->YPos = y;
			}
		}
	}

	void Maze::fixedUpdate()
	{
		for (auto wallVector : MazeTiles)
			for (auto walls : wallVector)
			{
				walls->fixedUpdate();
			}
	}


	Maze::~Maze()
	{
	}
}
