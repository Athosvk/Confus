#include "NewMaze.h"

namespace ConfusShared
{
	NewMaze::NewMaze(irr::core::vector2d<irr::u32> a_MazeSize)
		:m_MazeSize(a_MazeSize)
	{
		resetMaze();
	}

	void NewMaze::resetMaze()
	{
		MazeTiles.clear();
		for (unsigned int x = 0; x < m_MazeSize.X; x++)
		{
			std::vector<std::shared_ptr<NewMazeTile>> tempVector;
			MazeTiles.push_back(tempVector);
			for (unsigned int y = 0; y < m_MazeSize.Y; y++)
			{
				std::shared_ptr<NewMazeTile> mazeTile = std::make_shared<NewMazeTile>();
				MazeTiles[x].push_back(mazeTile);
				MazeTiles[x][y]->XPos = x;
				MazeTiles[x][y]->YPos = y;
			}
		}
	}

	irr::core::vector2d<irr::u32> const & NewMaze::mazeSize() const
	{
		return m_MazeSize;
	}


	NewMaze::~NewMaze()
	{
	}
}
