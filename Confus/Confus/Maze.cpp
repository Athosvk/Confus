#include "Maze.h"
#include "WalledMazeTile.h"

namespace Confus
{
	Maze::Maze(irr::IrrlichtDevice* a_Device, int a_MazeSizeX, int a_MazeSizeY, irr::core::vector3df a_StartPosition,float a_PathScalar, bool a_NeedRender)
		:m_MazeSizeX(a_MazeSizeX), m_MazeSizeY(a_MazeSizeY), m_PathScalar(a_PathScalar)
	{
		m_IrrDevice = a_Device;
		resetMaze(irr::core::vector2df(30, -7), a_NeedRender);
	}

	void Maze::resetMaze(irr::core::vector2df a_Offset, bool a_NeedRender)
	{
		MazeTiles.clear();
		for (int x = 0; x < m_MazeSizeX; x++)
		{
			std::vector<std::shared_ptr<MazeTile>> tempVector;
			MazeTiles.push_back(tempVector);
			for (int y = 0; y < m_MazeSizeY - 1; y++)
			{
				if (a_NeedRender)
				{
					std::shared_ptr<WalledMazeTile> mazeTile = std::make_shared<WalledMazeTile>(m_IrrDevice, irr::core::vector3df(static_cast<float>(-x*m_PathScalar + a_Offset.X), 0.5f, static_cast<float>(-y*m_PathScalar + a_Offset.Y)),
															 irr::core::vector3df(static_cast<float>(-x*m_PathScalar + a_Offset.X), 0.5f, static_cast<float>(-y*m_PathScalar + a_Offset.Y)));
					irr::scene::IAnimatedMeshSceneNode* wallMeshNode = mazeTile->getWall()->getMeshNode();
					irr::core::vector3df boundingBox = wallMeshNode->getBoundingBox().getExtent();
					irr::core::vector3df scalar = irr::core::vector3df(m_PathScalar, 1., m_PathScalar);//do not want to raise the wall height
					wallMeshNode->setScale(scalar);
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

	int const & Maze::mazeSizeY() const
	{
		return m_MazeSizeY;
	}

	int const & Maze::mazeSizeX() const
	{
		return m_MazeSizeX;
	}


	Maze::~Maze()
	{
	}
}
