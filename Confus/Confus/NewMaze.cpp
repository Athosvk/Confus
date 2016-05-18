#include "NewMaze.h"

namespace Confus
{
	NewMaze::NewMaze(int a_MazeSizeX, int a_MazeSizeY)
		:m_MazeSizeX(a_MazeSizeX), m_MazeSizeY(a_MazeSizeY)
	{
		resetMaze();
	}

	void NewMaze::resetMaze()
	{
		MazeTiles.clear();
		for (int x = 0; x < m_MazeSizeX; x++)
		{
			std::vector<std::shared_ptr<NewMazeTile>> tempVector;
			MazeTiles.push_back(tempVector);
			for (int y = 0; y < m_MazeSizeY - 1; y++)
			{
				std::shared_ptr<NewMazeTile> mazeTile = std::make_shared<NewMazeTile>();
				MazeTiles[x].push_back(mazeTile);
				MazeTiles[x][y]->XPos = x;
				MazeTiles[x][y]->YPos = y;
			}
		}
	}

	int const & NewMaze::mazeSizeY() const
	{
		return m_MazeSizeY;
	}

	int const & NewMaze::mazeSizeX() const
	{
		return m_MazeSizeX;
	}


	NewMaze::~NewMaze()
	{
	}
}
