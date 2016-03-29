#include "Maze.h"
#include "WalledMazeTile.h"

namespace Confus
{
	Maze::Maze(irr::IrrlichtDevice* a_Device, irr::core::vector3df a_StartPosition, bool a_NeedRender)
		:m_mazeSizeX(123), m_mazeSizeY(80)
	{
		m_irrDevice = a_Device;
		resetMaze(irr::core::vector2df(30, -7), a_NeedRender);
	}

	void Maze::resetMaze(irr::core::vector2df a_Offset, bool a_NeedRender)
	{
		MazeTiles.clear();
		for (int x = 0; x < m_mazeSizeX; x++)
		{
			std::vector<std::shared_ptr<MazeTile>> tempVector;
			MazeTiles.push_back(tempVector);
			for (int y = 0; y < m_mazeSizeY - 1; y++)
			{
				if (a_NeedRender)
				{
					std::shared_ptr<WalledMazeTile> mazeTile = std::make_shared<WalledMazeTile>(m_irrDevice, irr::core::vector3df(static_cast<float>(-x + a_Offset.X), 0.5f, static_cast<float>(-y + a_Offset.Y)),
															 irr::core::vector3df(static_cast<float>(-x + a_Offset.X), 0.5f, static_cast<float>(-y + a_Offset.Y)));
					const irr::scene::IAnimatedMeshSceneNode* wallMeshNode = mazeTile->getWall()->getMeshNode();
					irr::core::vector3df boundingBox = wallMeshNode->getBoundingBox().getExtent();
					mazeTile->getWall()->HiddenPosition = irr::core::vector3df(mazeTile->getWall()->HiddenPosition.X, -boundingBox.Y * wallMeshNode->getScale().Y, mazeTile->getWall()->HiddenPosition.Z);
					mazeTile->getWall()->TransitionSpeed = 0.5f;
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
		{
			for (auto wall : wallVector)
			{
				wall->fixedUpdate();
			}
		}
	}

	int const & Maze::MazeSizeY() const
	{
		return m_mazeSizeY;
	}

	int const & Maze::MazeSizeX() const
	{
		return m_mazeSizeX;
	}


	Maze::~Maze()
	{
	}
}
