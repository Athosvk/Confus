#include "Maze.h"

namespace Confus
{

	Maze::Maze(irr::IrrlichtDevice* a_Device, irr::core::vector3df a_StartPosition)
		:MazeSizeX(4), MazeSizeY(4)
	{
		irr::core::vector2df offset = irr::core::vector2df(30,-7);
		for (int x = 0; x < MazeSizeX; x++)
		{
			srand( 19 + 20 + 21 + 22 + 23 + 24);
			std::vector<MazeTile> tempVector;
			MazeTiles.push_back(tempVector);
			for (int y = 0; y < MazeSizeY-1; y++)
			{
				MazeTile mazeTile(a_Device,irr::core::vector3df(static_cast<float>(-x + offset.X),0.0f, static_cast<float>(-y + offset.Y)), irr::core::vector3df(static_cast<float>(-x + offset.X),0.0f, static_cast<float>(-y + offset.Y)));
				const irr::scene::IAnimatedMeshSceneNode* wallMeshNode = mazeTile.Wall.getMeshNode();
				irr::core::vector3df boundingBox = wallMeshNode->getBoundingBox().getExtent();
				mazeTile.Wall.HiddenPosition = irr::core::vector3df(mazeTile.Wall.HiddenPosition.X, -boundingBox.Y * wallMeshNode->getScale().Y, mazeTile.Wall.HiddenPosition.Z);
				mazeTile.Wall.TransitionSpeed = 0.5f;
				MazeTiles[x].push_back(mazeTile);
				MazeTiles[x][y].XPos = x;
				MazeTiles[x][y].YPos = y;
			}
		}
	}

	void Maze::fixedUpdate()
	{
		for (auto wallVector : MazeTiles)
			for (auto& walls : wallVector)
			{
				walls.Wall.fixedUpdate();
			}
	}


	Maze::~Maze()
	{
	}
}
