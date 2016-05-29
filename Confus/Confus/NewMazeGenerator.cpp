#include "NewMazeGenerator.h"

namespace Confus
{
	NewMazeGenerator::NewMazeGenerator()
	{
	}

	void NewMazeGenerator::generateMaze(NewMaze &  a_Maze, int a_Seed, irr::core::vector2df a_GenerateStartPoint)
	{
		//setup globals & variables
		srand(a_Seed);
		std::shared_ptr<NewMazeTile> currentTile = a_Maze.MazeTiles[static_cast<int>(a_GenerateStartPoint.X)][static_cast<int>(a_GenerateStartPoint.Y)];

		//Startcell must be set to visited, add to visitedcount
		currentTile->Raised = false;

		do
		{
			std::vector<std::shared_ptr<NewMazeTile>> Neighbours = getNeighbours(a_Maze.MazeTiles, *currentTile);
			if (Neighbours.size() != 0)
			{
				m_TileStack.push(currentTile);
				std::shared_ptr<NewMazeTile> tile = Neighbours[rand() % Neighbours.size()];
				int xmovement = (tile->XPos - currentTile->XPos) / 2;
				int ymovement = (tile->YPos - currentTile->YPos) / 2;

				if (xmovement != 0 || ymovement != 0)
				{
					std::shared_ptr<NewMazeTile>  inBetweenTile = a_Maze.MazeTiles[currentTile->XPos + xmovement][currentTile->YPos + ymovement];
					inBetweenTile->Raised = false;
				}

				currentTile = tile;
				currentTile->Raised = false;
			}
			else if (m_TileStack.size() != 0)
			{
				currentTile = m_TileStack.top();
				m_TileStack.pop();
			}

		} while (m_TileStack.size() != 0);
	}

	std::vector<std::shared_ptr<NewMazeTile>> NewMazeGenerator::getNeighbours(std::vector<std::vector<std::shared_ptr<NewMazeTile>>> & a_Maze, NewMazeTile& a_Tile)
	{
		std::vector<std::shared_ptr<NewMazeTile>> neighbours;
		size_t x = static_cast<size_t>(a_Tile.XPos);
		size_t y = static_cast<size_t>(a_Tile.YPos);


		//check if the neighbour is not out of bounds, and is not visited, else add to neighbours
		if (x > 1 && a_Maze[x - 2][y]->Raised)
		{
			neighbours.push_back(a_Maze[x - 2][y]);
		}
		if (x < a_Maze.size() - 2 && a_Maze[x + 2][y]->Raised)
		{
			neighbours.push_back(a_Maze[x + 2][y]);
		}
		if (y > 1 && a_Maze[x][y - 2]->Raised)
		{
			neighbours.push_back(a_Maze[x][y - 2]);
		}
		if (y < a_Maze[0].size() - 2 && a_Maze[x][y + 2]->Raised)
		{
			neighbours.push_back(a_Maze[x][y + 2]);
		}

		return neighbours;
	}
}
