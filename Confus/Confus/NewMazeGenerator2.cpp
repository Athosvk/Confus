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
		std::shared_ptr<NewMazeTile> currentTile = SetTileDown(a_Maze, a_GenerateStartPoint);

		do
		{
			std::vector<std::shared_ptr<NewMazeTile>> Neighbours = getNeighbours(a_Maze, *currentTile);
			if (Neighbours.size() != 0)
			{
				std::shared_ptr<NewMazeTile> tile = Neighbours[rand() % Neighbours.size()];
				irr::core::vector2d<irr::u32> tilePosition = irr::core::vector2d<irr::u32>(tile->XPos, tile->YPos);
				int xmovement = (tile->XPos - currentTile->XPos);
				int ymovement = (tile->YPos - currentTile->YPos);

				if (xmovement < 0)
				{
					SetTileLeft(a_Maze, tilePosition);
				}
				else if (xmovement > 0)
				{
					SetTileRight(a_Maze, tilePosition);
				}
				else if (ymovement < 0)
				{
					SetTileUp(a_Maze, tilePosition);
				}
				else if (ymovement > 0)
				{
					SetTileDown(a_Maze, tilePosition);
				}
			}
			else if (m_TileStack.size() != 0)
			{
				currentTile = m_TileStack.top();
				m_TileStack.pop();
			}

		} while (m_TileStack.size() != 0);
	}

	NewMazeGenerator2::~NewMazeGenerator2()
	{
	}

	std::vector<std::shared_ptr<NewMazeTile>> NewMazeGenerator2::getNeighbours(NewMaze &  a_Maze, NewMazeTile& a_Tile)
	{
		std::vector<std::shared_ptr<NewMazeTile>> neighbours;
		size_t x = static_cast<size_t>(a_Tile.XPos);
		size_t y = static_cast<size_t>(a_Tile.YPos);


		//check if the neighbour is not out of bounds, and is not visited, else add to neighbours
		if (CheckLeftNeighbours(a_Maze, a_Tile))
		{
			neighbours.push_back(a_Maze.MazeTiles[x - 1][y]);
		}
		if (CheckRightNeighbours(a_Maze, a_Tile))
		{
			neighbours.push_back(a_Maze.MazeTiles[x + 1][y]);
		}
		if (CheckUpNeighbours(a_Maze, a_Tile))
		{
			neighbours.push_back(a_Maze.MazeTiles[x][y - 1]);
		}
		if (CheckDownNeighbours(a_Maze, a_Tile))
		{
			neighbours.push_back(a_Maze.MazeTiles[x][y + 1]);
		}

		return neighbours;
	}

	bool NewMazeGenerator2::CheckLeftNeighbours(NewMaze &  a_Maze, NewMazeTile& a_Tile)
	{
		size_t x = static_cast<size_t>(a_Tile.XPos);
		size_t y = static_cast<size_t>(a_Tile.YPos);

		if (x > 2 && y + 1 < a_Maze.MazeTiles.size()) // is not out of bounds
		{
			if (a_Maze.MazeTiles[x - 1][y]->Raised && a_Maze.MazeTiles[x - 1][y + 1]->Raised) // tile next to it is not lowered
			{
				if (a_Maze.MazeTiles[x - 2][y]->Raised && a_Maze.MazeTiles[x - 2][y + 1]->Raised) // 2 tiles shift is not lowered
				{
					if (a_Maze.MazeTiles[x - 3][y]->Raised && a_Maze.MazeTiles[x - 3][y + 1]->Raised) // third tile shift is not lowered
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	bool NewMazeGenerator2::CheckRightNeighbours(NewMaze &  a_Maze, NewMazeTile& a_Tile)
	{
		size_t x = static_cast<size_t>(a_Tile.XPos);
		size_t y = static_cast<size_t>(a_Tile.YPos);

		if (x < a_Maze.MazeTiles.size()-3 && y+ 1 < a_Maze.MazeTiles.size()) // is not out of bounds
		{
			if (a_Maze.MazeTiles[x + 1][y]->Raised && a_Maze.MazeTiles[x + 1][y + 1]->Raised) // tile next to it is not lowered
			{
				if (a_Maze.MazeTiles[x + 2][y]->Raised && a_Maze.MazeTiles[x + 2][y + 1]->Raised) // 2 tiles shift is not lowered
				{
					if (a_Maze.MazeTiles[x + 3][y]->Raised && a_Maze.MazeTiles[x + 3][y + 1]->Raised) // third tile shift is not lowered
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	bool NewMazeGenerator2::CheckUpNeighbours(NewMaze &  a_Maze, NewMazeTile& a_Tile)
	{
		size_t x = static_cast<size_t>(a_Tile.XPos);
		size_t y = static_cast<size_t>(a_Tile.YPos);

		if (x + 1 < a_Maze.MazeTiles.size() && y > 2) // is not out of bounds
		{
			if (a_Maze.MazeTiles[x][y-1]->Raised && a_Maze.MazeTiles[x + 1][y - 1]->Raised) // tile next to it is not lowered
			{
				if (a_Maze.MazeTiles[x][y-2]->Raised && a_Maze.MazeTiles[x + 1][y - 2]->Raised) // 2 tiles shift is not lowered
				{
					if (a_Maze.MazeTiles[x][y-3]->Raised && a_Maze.MazeTiles[x + 1][y - 3]->Raised) // third tile shift is not lowered
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	bool NewMazeGenerator2::CheckDownNeighbours(NewMaze &  a_Maze, NewMazeTile& a_Tile)
	{
		size_t x = static_cast<size_t>(a_Tile.XPos);
		size_t y = static_cast<size_t>(a_Tile.YPos);

		if (x + 1 < a_Maze.MazeTiles.size() && y  < a_Maze.MazeTiles.size() - 3) // is not out of bounds
		{
			if (a_Maze.MazeTiles[x][y + 1]->Raised && a_Maze.MazeTiles[x + 1][y + 1]->Raised) // tile next to it is not lowered
			{
				if (a_Maze.MazeTiles[x][y + 2]->Raised && a_Maze.MazeTiles[x + 1][y + 2]->Raised) // 2 tiles shift is not lowered
				{
					if (a_Maze.MazeTiles[x][y + 3]->Raised && a_Maze.MazeTiles[x + 1][y + 3]->Raised) // third tile shift is not lowered
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	//for the sake of finishing stuff for the market, assuming generatestart is in the middle of the maze
	std::shared_ptr<NewMazeTile> NewMazeGenerator2::SetTileDown(NewMaze &  a_Maze, irr::core::vector2d<irr::u32> a_GenerateStartPoint)
	{
		SetTileVisited(a_Maze, a_GenerateStartPoint);
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(0, 1));
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(0, 2));
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(1, 0));
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(1, 1));
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(1, 2));
		irr::core::vector2d<irr::u32> temp = irr::core::vector2d<irr::u32>(a_GenerateStartPoint + irr::core::vector2d<irr::u32>(0, 2));
		return a_Maze.MazeTiles[temp.X][temp.Y];
	}

	std::shared_ptr<NewMazeTile> NewMazeGenerator2::SetTileUp(NewMaze &  a_Maze, irr::core::vector2d<irr::u32> a_GenerateStartPoint)
	{
		SetTileVisited(a_Maze, a_GenerateStartPoint);
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(0, -1));
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(0, -2));
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(1, 0));
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(1, -1));
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(1, -2));
		irr::core::vector2d<irr::u32> temp = irr::core::vector2d<irr::u32>(a_GenerateStartPoint + irr::core::vector2d<irr::u32>(0, -2));
		return a_Maze.MazeTiles[temp.X][temp.Y];
	}

	std::shared_ptr<NewMazeTile> NewMazeGenerator2::SetTileRight(NewMaze &  a_Maze, irr::core::vector2d<irr::u32> a_GenerateStartPoint)
	{
		SetTileVisited(a_Maze, a_GenerateStartPoint);
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(1, 0));
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(2, 0));
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(0, 1));
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(1, 1));
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(2, 1));
		irr::core::vector2d<irr::u32> temp = irr::core::vector2d<irr::u32>(a_GenerateStartPoint + irr::core::vector2d<irr::u32>(2, 0));
		return a_Maze.MazeTiles[temp.X][temp.Y];
	}

	std::shared_ptr<NewMazeTile> NewMazeGenerator2::SetTileLeft(NewMaze &  a_Maze, irr::core::vector2d<irr::u32> a_GenerateStartPoint)
	{
		SetTileVisited(a_Maze, a_GenerateStartPoint);
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(-1, 0));
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(-2, 0));
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(0, 1));
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(-1, 1));
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(-2, 1));
		irr::core::vector2d<irr::u32> temp = irr::core::vector2d<irr::u32>(a_GenerateStartPoint + irr::core::vector2d<irr::u32>(-2, 0));
		return a_Maze.MazeTiles[temp.X][temp.Y];
	}

	void NewMazeGenerator2::SetTileVisited(NewMaze &  a_Maze, irr::core::vector2d<irr::u32> a_Position)
	{
		std::shared_ptr<NewMazeTile> tile = a_Maze.MazeTiles[a_Position.X][a_Position.Y];
		tile->Raised = false;
		m_TileStack.push(tile);
	}
	
}
