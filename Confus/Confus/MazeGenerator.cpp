#include<iostream>

#include "MazeGenerator.h"
#include "WalledMazeTile.h"

namespace Confus
{

	MazeGenerator::MazeGenerator(irr::IrrlichtDevice* a_Device, irr::core::vector3df a_StartPosition, int a_InitialSeed)
		: MainMaze(a_Device, a_StartPosition,true), ReplacementMaze(a_Device, a_StartPosition, false), Seed(a_InitialSeed)
	{
		generateMaze(MainMaze.MazeTiles, a_InitialSeed);
		MainMaze.Identifier = 1;
		ReplacementMaze.Identifier = 2;
	}

	void MazeGenerator::fixedUpdate()
	{
		MainMaze.fixedUpdate();
	}

	void MazeGenerator::refillMainMaze(int a_Seed)
	{
		generateMaze(ReplacementMaze.MazeTiles, a_Seed);
		MainMaze = ReplacementMaze;
		ReplacementMaze.resetMaze(irr::core::vector2df(30,-7),false);
	}

	void MazeGenerator::generateMaze(std::vector<std::vector<std::shared_ptr<MazeTile>>> &  a_Maze, int a_Seed)
	{
		//setup globals & variables
		srand(a_Seed);
		MoveableWall* wall = 0;
		std::shared_ptr<MazeTile> CurrentTile = a_Maze[0][0];

		//Startcell must be set to visited, add to visitedcount
		CurrentTile->Raised = false;

		wall = CurrentTile->getWall();
		if (wall)
		{
			wall->hide();
		}

		do
		{
			std::vector<std::shared_ptr<MazeTile>> Neighbours = getNeighbours(a_Maze,*CurrentTile);
			if (Neighbours.size() != 0)
			{
				TileStack.push(CurrentTile);
				std::shared_ptr<MazeTile> tile = Neighbours[rand() % Neighbours.size()];
				int xmovement = (tile->XPos - CurrentTile->XPos) / 2;
				int ymovement = (tile->YPos - CurrentTile->YPos) / 2;


				if (xmovement != 0)
				{
					std::shared_ptr<MazeTile>  inBetweenTile = a_Maze[CurrentTile->XPos + xmovement][CurrentTile->YPos];
					inBetweenTile->Raised = false;
					wall = inBetweenTile->getWall();
					if (wall)
					{
						wall->hide();
					}
				}
				else if (ymovement != 0)
				{
					std::shared_ptr<MazeTile> inBetweenTile = a_Maze[CurrentTile->XPos][CurrentTile->YPos + ymovement];
					inBetweenTile->Raised = false;
					wall = inBetweenTile->getWall();
					if (wall)
					{
						wall->hide();
					}
				}

				CurrentTile = tile;

				CurrentTile->Raised = false;
				wall = CurrentTile->getWall();
				if (wall)
				{
					wall->hide();
				}
			}
			else if (TileStack.size() != 0)
			{
				CurrentTile = TileStack.top();
				TileStack.pop();
			}

		} while (TileStack.size() != 0);
	}

	std::vector<std::shared_ptr<MazeTile>> MazeGenerator::getNeighbours(std::vector<std::vector<std::shared_ptr<MazeTile>>> & a_Maze, MazeTile& a_Tile)
	{
		std::vector<std::shared_ptr<MazeTile>> Neighbours;
		size_t x = static_cast<size_t>(a_Tile.XPos);
		size_t y = static_cast<size_t>(a_Tile.YPos);


		//check if the neighbour is not out of bounds, and is not visited, else add to neighbours
		if (x > 1 && a_Maze[x - 2][y]->Raised)
		{
			Neighbours.push_back(a_Maze[x - 2][y]);
		}
		if (x < a_Maze.size() - 2 && a_Maze[x + 2][y]->Raised)
		{
			Neighbours.push_back(a_Maze[x + 2][y]);
		}
		if (y > 1 && a_Maze[x][y - 2]->Raised)
		{
			Neighbours.push_back(a_Maze[x][y - 2]);
		}
		if (y < a_Maze[0].size() - 2 && a_Maze[x][y + 2]->Raised)
		{
			Neighbours.push_back(a_Maze[x][y + 2]);
		}

		return Neighbours;
	}

	MazeGenerator::~MazeGenerator()
	{
	}
}
