#include "NewMazeGenerator2.h"

namespace Confus
{
	NewMazeGenerator2::NewMazeGenerator2()
	{
	}

	void NewMazeGenerator2::generateMaze(NewMaze &  a_Maze, int a_Seed, irr::core::vector2d<irr::u32> a_GenerateStartPoint)
	{
		//setup globals & variables
		srand(a_Seed);
		std::shared_ptr<NewMazeTile> currentTile = a_Maze.MazeTiles[a_GenerateStartPoint.X][a_GenerateStartPoint.Y];

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

	std::vector<std::shared_ptr<NewMazeTile>> NewMazeGenerator2::getNeighbours(std::vector<std::vector<std::shared_ptr<NewMazeTile>>> & a_Maze, NewMazeTile& a_Tile)
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

	std::shared_ptr<NewMazeTile> NewMazeGenerator2::CheckLeftNeighbours(NewMaze &  a_Maze, NewMazeTile& a_Tile)
	{
		size_t x = static_cast<size_t>(a_Tile.XPos);
		size_t y = static_cast<size_t>(a_Tile.YPos);

		if (x > 2 && y < a_Maze.MazeTiles.size()) // is not out of bounds
		{
			if (a_Maze.MazeTiles[x - 1][y]->Raised && a_Maze.MazeTiles[x - 1][y + 1]->Raised) // tile next to it is not lowered
			{
				if (a_Maze.MazeTiles[x - 2][y]->Raised && a_Maze.MazeTiles[x - 2][y + 1]->Raised) // 2 tiles shift is not lowered
				{
					if (a_Maze.MazeTiles[x - 3][y]->Raised && a_Maze.MazeTiles[x - 3][y + 1]->Raised) // third tile shift is not lowered
					{
						return a_Maze.MazeTiles[x - 1][y];
					}
				}
			}
		}
	}

	std::shared_ptr<NewMazeTile> NewMazeGenerator2::CheckRightNeighbours(NewMaze &  a_Maze, NewMazeTile& a_Tile)
	{
		size_t x = static_cast<size_t>(a_Tile.XPos);
		size_t y = static_cast<size_t>(a_Tile.YPos);

		if (x < a_Maze.MazeTiles.size()-2 && y < a_Maze.MazeTiles.size()) // is not out of bounds
		{
			if (a_Maze.MazeTiles[x + 1][y]->Raised && a_Maze.MazeTiles[x + 1][y + 1]->Raised) // tile next to it is not lowered
			{
				if (a_Maze.MazeTiles[x + 2][y]->Raised && a_Maze.MazeTiles[x + 2][y + 1]->Raised) // 2 tiles shift is not lowered
				{
					if (a_Maze.MazeTiles[x + 3][y]->Raised && a_Maze.MazeTiles[x + 3][y + 1]->Raised) // third tile shift is not lowered
					{
						return a_Maze.MazeTiles[x + 1][y];
					}
				}
			}
		}
	}

	std::shared_ptr<NewMazeTile> NewMazeGenerator2::CheckDownNeighbours(NewMaze &  a_Maze, NewMazeTile& a_Tile)
	{
		size_t x = static_cast<size_t>(a_Tile.XPos);
		size_t y = static_cast<size_t>(a_Tile.YPos);

		if (x < a_Maze.MazeTiles.size() && y < a_Maze.MazeTiles.size() - 2) // is not out of bounds
		{
			if (a_Maze.MazeTiles[x][y-1]->Raised && a_Maze.MazeTiles[x + 1][y - 1]->Raised) // tile next to it is not lowered
			{
				if (a_Maze.MazeTiles[x][y-2]->Raised && a_Maze.MazeTiles[x + 1][y - 2]->Raised) // 2 tiles shift is not lowered
				{
					if (a_Maze.MazeTiles[x][y-3]->Raised && a_Maze.MazeTiles[x + 1][y - 3]->Raised) // third tile shift is not lowered
					{
						return a_Maze.MazeTiles[x][y];
					}
				}
			}
		}
	}

	std::shared_ptr<NewMazeTile> NewMazeGenerator2::CheckUpNeighbours(NewMaze &  a_Maze, NewMazeTile& a_Tile)
	{
		size_t x = static_cast<size_t>(a_Tile.XPos);
		size_t y = static_cast<size_t>(a_Tile.YPos);

		if (x < a_Maze.MazeTiles.size() && y > 2) // is not out of bounds
		{
			if (a_Maze.MazeTiles[x][y + 1]->Raised && a_Maze.MazeTiles[x + 1][y + 1]->Raised) // tile next to it is not lowered
			{
				if (a_Maze.MazeTiles[x][y + 2]->Raised && a_Maze.MazeTiles[x + 1][y + 2]->Raised) // 2 tiles shift is not lowered
				{
					if (a_Maze.MazeTiles[x][y + 3]->Raised && a_Maze.MazeTiles[x + 1][y + 3]->Raised) // third tile shift is not lowered
					{
						return a_Maze.MazeTiles[x][y];
					}
				}
			}
		}
	}

	//for the sake of finishing stuff for the market, assuming generatestart is in the middle of the maze
	void NewMazeGenerator2::SetTileDown(NewMaze &  a_Maze, irr::core::vector2d<irr::u32> a_GenerateStartPoint)
	{
		SetTileVisited(a_Maze, a_GenerateStartPoint);
		SetTileVisited(a_Maze, a_GenerateStartPoint - irr::core::vector2d<irr::u32>(0, 1));
		SetTileVisited(a_Maze, a_GenerateStartPoint - irr::core::vector2d<irr::u32>(0, 2));
		SetTileVisited(a_Maze, a_GenerateStartPoint - irr::core::vector2d<irr::u32>(1, 0));
		SetTileVisited(a_Maze, a_GenerateStartPoint - irr::core::vector2d<irr::u32>(1, 1));
		SetTileVisited(a_Maze, a_GenerateStartPoint - irr::core::vector2d<irr::u32>(1, 2));
	}

	void NewMazeGenerator2::SetTileUp(NewMaze &  a_Maze, irr::core::vector2d<irr::u32> a_GenerateStartPoint)
	{
		SetTileVisited(a_Maze, a_GenerateStartPoint);
		SetTileVisited(a_Maze, a_GenerateStartPoint - irr::core::vector2d<irr::u32>(0, -1));
		SetTileVisited(a_Maze, a_GenerateStartPoint - irr::core::vector2d<irr::u32>(0, -2));
		SetTileVisited(a_Maze, a_GenerateStartPoint - irr::core::vector2d<irr::u32>(1, 0));
		SetTileVisited(a_Maze, a_GenerateStartPoint - irr::core::vector2d<irr::u32>(1, -1));
		SetTileVisited(a_Maze, a_GenerateStartPoint - irr::core::vector2d<irr::u32>(1, -2));
	}

	void NewMazeGenerator2::SetTileRight(NewMaze &  a_Maze, irr::core::vector2d<irr::u32> a_GenerateStartPoint)
	{
		SetTileVisited(a_Maze, a_GenerateStartPoint);
		SetTileVisited(a_Maze, a_GenerateStartPoint - irr::core::vector2d<irr::u32>(1, 0));
		SetTileVisited(a_Maze, a_GenerateStartPoint - irr::core::vector2d<irr::u32>(2, 0));
		SetTileVisited(a_Maze, a_GenerateStartPoint - irr::core::vector2d<irr::u32>(0, 1));
		SetTileVisited(a_Maze, a_GenerateStartPoint - irr::core::vector2d<irr::u32>(1, 1));
		SetTileVisited(a_Maze, a_GenerateStartPoint - irr::core::vector2d<irr::u32>(2, 1));
	}

	void NewMazeGenerator2::SetTileLeft(NewMaze &  a_Maze, irr::core::vector2d<irr::u32> a_GenerateStartPoint)
	{
		SetTileVisited(a_Maze, a_GenerateStartPoint);
		SetTileVisited(a_Maze, a_GenerateStartPoint - irr::core::vector2d<irr::u32>(-1, 0));
		SetTileVisited(a_Maze, a_GenerateStartPoint - irr::core::vector2d<irr::u32>(-2, 0));
		SetTileVisited(a_Maze, a_GenerateStartPoint - irr::core::vector2d<irr::u32>(0, 1));
		SetTileVisited(a_Maze, a_GenerateStartPoint - irr::core::vector2d<irr::u32>(-1, 1));
		SetTileVisited(a_Maze, a_GenerateStartPoint - irr::core::vector2d<irr::u32>(-2, 1));
	}

	void NewMazeGenerator2::SetTileVisited(NewMaze &  a_Maze, irr::core::vector2d<irr::u32> a_Position)
	{
		std::shared_ptr<NewMazeTile> tile = a_Maze.MazeTiles[a_Position.X][a_Position.Y];
		tile->Raised = false;
		m_TileStack.push(tile);
	}
	
}
