#include "NewMazeGenerator2.h"

namespace ConfusShared
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

		ForceEntrancesDown(a_Maze);
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

		if (x > 3 && y + 1 < a_Maze.MazeTiles[0].size()) // is not out of bounds
		{
			if (a_Maze.MazeTiles[x - 1][y]->Raised && a_Maze.MazeTiles[x - 1][y + 1]->Raised && (y + 2 == a_Maze.MazeTiles[0].size() || a_Maze.MazeTiles[x - 1][y + 2]->Raised) && (y == 0 || a_Maze.MazeTiles[x - 1][y - 1]->Raised)) // tile next to it is not lowered
			{
				if (a_Maze.MazeTiles[x - 2][y]->Raised && a_Maze.MazeTiles[x - 2][y + 1]->Raised && (y + 2 == a_Maze.MazeTiles[0].size() || a_Maze.MazeTiles[x - 2][y + 2]->Raised) && (y == 0 || a_Maze.MazeTiles[x - 2][y - 1]->Raised)) // 2 tiles shift is not lowered
				{
					if (a_Maze.MazeTiles[x - 3][y]->Raised && a_Maze.MazeTiles[x - 3][y + 1]->Raised && (y + 2 == a_Maze.MazeTiles[0].size() || a_Maze.MazeTiles[x - 3][y + 2]->Raised) && (y == 0 || a_Maze.MazeTiles[x - 3][y - 1]->Raised)) // third tile shift is not lowered
					{
						if (a_Maze.MazeTiles[x - 4][y]->Raised && a_Maze.MazeTiles[x - 4][y + 1]->Raised) //fourth tile is not forbidden
						{
							return true;
						}
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

		if (x < a_Maze.MazeTiles.size()-4 && y+ 1 < a_Maze.MazeTiles[0].size()) // is not out of bounds
		{
			if (a_Maze.MazeTiles[x + 1][y]->Raised && a_Maze.MazeTiles[x + 1][y + 1]->Raised && (y + 2 == a_Maze.MazeTiles[0].size() || a_Maze.MazeTiles[x + 1][y + 2]->Raised) && (y == 0 || a_Maze.MazeTiles[x + 1][y - 1]->Raised)) // tile next to it is not lowered
			{
				if (a_Maze.MazeTiles[x + 2][y]->Raised && a_Maze.MazeTiles[x + 2][y + 1]->Raised && (y + 2 == a_Maze.MazeTiles[0].size() || a_Maze.MazeTiles[x + 2][y + 2]->Raised) && (y == 0 || a_Maze.MazeTiles[x + 2][y - 1]->Raised)) // 2 tiles shift is not lowered
				{
					if (a_Maze.MazeTiles[x + 3][y]->Raised && a_Maze.MazeTiles[x + 3][y + 1]->Raised && (y + 2 == a_Maze.MazeTiles[0].size() || a_Maze.MazeTiles[x + 3][y + 2]->Raised) && (y == 0 || a_Maze.MazeTiles[x + 3][y - 1]->Raised)) // third tile shift is not lowered
					{
						if (a_Maze.MazeTiles[x + 4][y]->Raised && a_Maze.MazeTiles[x + 4][y + 1]->Raised) //fourth tile is not forbidden
						{
							return true;
						}
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

		if (x + 1 < a_Maze.MazeTiles.size() && y > 3) // is not out of bounds
		{
			if (a_Maze.MazeTiles[x][y-1]->Raised && a_Maze.MazeTiles[x + 1][y - 1]->Raised && (x + 2 == a_Maze.MazeTiles.size() || a_Maze.MazeTiles[x + 2][y - 1]->Raised) && (x == 0 || a_Maze.MazeTiles[x - 1][y - 1]->Raised)) // tile next to it is not lowered
			{
				if (a_Maze.MazeTiles[x][y-2]->Raised && a_Maze.MazeTiles[x + 1][y - 2]->Raised && (x + 2 == a_Maze.MazeTiles.size() || a_Maze.MazeTiles[x + 2][y - 2]->Raised) && (x == 0 || a_Maze.MazeTiles[x - 1][y - 2]->Raised)) // 2 tiles shift is not lowered
				{
					if (a_Maze.MazeTiles[x][y-3]->Raised && a_Maze.MazeTiles[x + 1][y - 3]->Raised && (x + 2 == a_Maze.MazeTiles.size() || a_Maze.MazeTiles[x + 2][y - 3]->Raised) && (x == 0 || a_Maze.MazeTiles[x - 1][y - 3]->Raised)) // third tile shift is not lowered
					{
						if (a_Maze.MazeTiles[x][y - 4]->Raised && a_Maze.MazeTiles[x + 1][y - 4]->Raised) //fourth tile is not forbidden
						{
							return true;
						}
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

		if (x + 1 < a_Maze.MazeTiles.size() && y  < a_Maze.MazeTiles[0].size() - 4) // is not out of bounds
		{
			if (a_Maze.MazeTiles[x][y + 1]->Raised && a_Maze.MazeTiles[x + 1][y + 1]->Raised && (x + 2 == a_Maze.MazeTiles.size() || a_Maze.MazeTiles[x + 2][y + 1]->Raised) && (x == 0 || a_Maze.MazeTiles[x - 1][y + 1]->Raised)) // tile next to it is not lowered
			{
				if (a_Maze.MazeTiles[x][y + 2]->Raised && a_Maze.MazeTiles[x + 1][y + 2]->Raised && (x + 2 == a_Maze.MazeTiles.size() || a_Maze.MazeTiles[x + 2][y + 2]->Raised) && (x == 0 || a_Maze.MazeTiles[x - 1][y + 2]->Raised)) // 2 tiles shift is not lowered
				{
					if (a_Maze.MazeTiles[x][y + 3]->Raised && a_Maze.MazeTiles[x + 1][y + 3]->Raised && (x + 2 == a_Maze.MazeTiles.size() || a_Maze.MazeTiles[x + 2][y + 3]->Raised) && (x == 0 || a_Maze.MazeTiles[x - 1][y + 3]->Raised)) // third tile shift is not lowered
					{
						if (a_Maze.MazeTiles[x][y + 4]->Raised && a_Maze.MazeTiles[x + 1][y + 4]->Raised) //fourth tile is not forbidden
						{
							return true;
						}
					}
				}
			}
		}
		return false;
	}

	//for the sake of finishing stuff for the market, assuming generatestart is in the middle of the maze
	std::shared_ptr<NewMazeTile> NewMazeGenerator2::SetTileDown(NewMaze &  a_Maze, irr::core::vector2d<irr::u32> a_GenerateStartPoint)
	{
		if (a_GenerateStartPoint.X != 0)
		{
			SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(-1, 0));
			SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(-1, 1));
			SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(-1, 2));
		}

		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(1, 0));
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(1, 1));
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(1, 2));

		SetTileVisited(a_Maze, a_GenerateStartPoint);
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(0, 1));
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(0, 2));

		if (a_GenerateStartPoint.X + 2 < a_Maze.MazeTiles[0].size() - 1)
		{
			SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(2, 0));
			SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(2, 1));
			SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(2, 2));
		}

		//top
		SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(0, 3));
		SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(1, 3));

		irr::core::vector2d<irr::u32> temp = irr::core::vector2d<irr::u32>(a_GenerateStartPoint + irr::core::vector2d<irr::u32>(0, 2));
		return a_Maze.MazeTiles[temp.X][temp.Y];
	}

	std::shared_ptr<NewMazeTile> NewMazeGenerator2::SetTileUp(NewMaze &  a_Maze, irr::core::vector2d<irr::u32> a_GenerateStartPoint)
	{
		if (a_GenerateStartPoint.X != 0)
		{
			SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(-1, 0));
			SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(-1, -1));
			SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(-1, -2));
		}

		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(1, 0));
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(1, -1));
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(1, -2));

		SetTileVisited(a_Maze, a_GenerateStartPoint);
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(0, -1));
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(0, -2));

		if (a_GenerateStartPoint.X + 2 < a_Maze.MazeTiles[0].size() - 1)
		{
			SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(2, 0));
			SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(2, -1));
			SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(2, -2));
		}
		//bottem

		SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(0, -3));
		SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(1, -3));

		irr::core::vector2d<irr::u32> temp = irr::core::vector2d<irr::u32>(a_GenerateStartPoint + irr::core::vector2d<irr::u32>(0, -2));
		return a_Maze.MazeTiles[temp.X][temp.Y];
	}

	std::shared_ptr<NewMazeTile> NewMazeGenerator2::SetTileRight(NewMaze &  a_Maze, irr::core::vector2d<irr::u32> a_GenerateStartPoint)
	{
		if (a_GenerateStartPoint.Y != 0)
		{
			SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(0, -1));
			SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(1, -1));
			SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(2, -1));
		}

		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(0, 1));
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(1, 1));
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(2, 1));

		SetTileVisited(a_Maze, a_GenerateStartPoint);
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(1, 0));
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(2, 0));

		if (a_GenerateStartPoint.Y + 2 < a_Maze.MazeTiles.size() -1 )
		{
			SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(0, 2));
			SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(1, 2));
			SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(2, 2));
		}

		//rightTop
		SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(3, 0));
		SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(3, 1));

		irr::core::vector2d<irr::u32> temp = irr::core::vector2d<irr::u32>(a_GenerateStartPoint + irr::core::vector2d<irr::u32>(2, 0));
		return a_Maze.MazeTiles[temp.X][temp.Y];
	}

	std::shared_ptr<NewMazeTile> NewMazeGenerator2::SetTileLeft(NewMaze &  a_Maze, irr::core::vector2d<irr::u32> a_GenerateStartPoint)
	{
		if (a_GenerateStartPoint.Y != 0)
		{
			SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(0, -1));
			SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(1, -1));
			SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(2, -1));
		}

		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(0, 1));
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(-1, 1));
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(-2, 1));

		SetTileVisited(a_Maze, a_GenerateStartPoint);
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(-1, 0));
		SetTileVisited(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(-2, 0));

		if (a_GenerateStartPoint.Y + 2 < a_Maze.MazeTiles.size() -1)
		{
			SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(0, 2));
			SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(-1, 2));
			SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(-2, 2));
		}

		//leftTop
		SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(-3, 0));
		SetTileWall(a_Maze, a_GenerateStartPoint + irr::core::vector2d<irr::u32>(-3, 1));

		irr::core::vector2d<irr::u32> temp = irr::core::vector2d<irr::u32>(a_GenerateStartPoint + irr::core::vector2d<irr::u32>(-2, 0));
		return a_Maze.MazeTiles[temp.X][temp.Y];
	}

	void NewMazeGenerator2::SetTileVisited(NewMaze &  a_Maze, irr::core::vector2d<irr::u32> a_Position)
	{
		std::shared_ptr<NewMazeTile> tile = a_Maze.MazeTiles[a_Position.X][a_Position.Y];
		tile->Raised = false;
		m_TileStack.push(tile);
	}

	void NewMazeGenerator2::SetTileWall(NewMaze &  a_Maze, irr::core::vector2d<irr::u32> a_Position)
	{
		std::shared_ptr<NewMazeTile> tile = a_Maze.MazeTiles[a_Position.X][a_Position.Y];
		tile->Wall = true;
	}

	void NewMazeGenerator2::ForceEntrancesDown(NewMaze &  a_Maze)
	{
		ForceWallsUp(a_Maze);
		for (int i = 0; i < 4; i++)
		{
			a_Maze.MazeTiles[11 + i][0]->Raised = false;
			a_Maze.MazeTiles[11 + i][1]->Raised = false;
		}
		for (int i = 0; i < 5; i++)
		{
			a_Maze.MazeTiles[18 + i][0]->Raised = false;
			a_Maze.MazeTiles[18 + i][1]->Raised = false;
		}
		for (int i = 0; i < 4; i++)
		{
			a_Maze.MazeTiles[26 + i][0]->Raised = false;
			a_Maze.MazeTiles[26 + i][1]->Raised = false;
		}

		for (int i = 0; i < 4; i++)
		{
			a_Maze.MazeTiles[11 + i][a_Maze.MazeTiles[0].size() - 1]->Raised = false;
			a_Maze.MazeTiles[11 + i][a_Maze.MazeTiles[0].size() - 2]->Raised = false;
		}
		for (int i = 0; i < 5; i++)
		{
			a_Maze.MazeTiles[18 + i][a_Maze.MazeTiles[0].size() - 1]->Raised = false;
			a_Maze.MazeTiles[18 + i][a_Maze.MazeTiles[0].size() - 2]->Raised = false;
		}
		for (int i = 0; i < 4; i++)
		{
			a_Maze.MazeTiles[26 + i][a_Maze.MazeTiles[0].size() - 1]->Raised = false;
			a_Maze.MazeTiles[26 + i][a_Maze.MazeTiles[0].size() - 2]->Raised = false;
		}
	}

	void NewMazeGenerator2::ForceWallsUp(NewMaze & a_Maze)
	{
		for (size_t x = 0; x < a_Maze.MazeTiles.size(); x++)
		{
			for (size_t y = 0; y < a_Maze.MazeTiles[0].size(); y++)
			{
				if ( x == 0 || y == 0 || x == a_Maze.MazeTiles.size() - 1 || y == a_Maze.MazeTiles[0].size()-1)
					a_Maze.MazeTiles[x][y]->Raised = true;
			}
		}
	}
	
}
