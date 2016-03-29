#include<iostream>

#include "MazeGenerator.h"
#include "WalledMazeTile.h"

namespace Confus
{

	MazeGenerator::MazeGenerator(irr::IrrlichtDevice* a_Device, irr::core::vector3df a_StartPosition, int a_InitialSeed)
		: m_mainMaze(a_Device, a_StartPosition,true), m_replacementMaze(a_Device, a_StartPosition, false), m_seed(a_InitialSeed)
	{
		generateMaze(m_mainMaze.MazeTiles, a_InitialSeed);
	}

	void MazeGenerator::fixedUpdate()
	{
		m_mainMaze.fixedUpdate();
	}

	void MazeGenerator::refillMainMaze(int a_Seed)
	{
		generateMaze(m_replacementMaze.MazeTiles, a_Seed);
		replaceMainMaze();
		m_replacementMaze.resetMaze(irr::core::vector2df(30,-7),false);
	}

	void MazeGenerator::replaceMainMaze()
	{
		for (int x = 0; x < m_mainMaze.MazeSizeX(); x++)
		{
			for (int y = 0; y < m_mainMaze.MazeSizeY() - 1; y++)
			{
				if (m_replacementMaze.MazeTiles[x][y]->Raised)
				{
					if (!m_mainMaze.MazeTiles[x][y]->Raised)
					{
						m_mainMaze.MazeTiles[x][y]->Raised = true;
						m_mainMaze.MazeTiles[x][y]->getWall()->rise();
					}
				}
				else
				{
					if (m_mainMaze.MazeTiles[x][y]->Raised)
					{
						m_mainMaze.MazeTiles[x][y]->Raised = false;
						m_mainMaze.MazeTiles[x][y]->getWall()->hide();
					}
				}
			}
		}
	}

	void MazeGenerator::generateMaze(std::vector<std::vector<std::shared_ptr<MazeTile>>> &  a_Maze, int a_Seed)
	{
		//setup globals & variables
		srand(a_Seed);
		MoveableWall* wall = nullptr;
		std::shared_ptr<MazeTile> currentTile = a_Maze[0][0];

		//Startcell must be set to visited, add to visitedcount
		currentTile->Raised = false;

		wall = currentTile->getWall();
		if (wall)
		{
			wall->hide();
		}

		do
		{
			std::vector<std::shared_ptr<MazeTile>> Neighbours = getNeighbours(a_Maze,*currentTile);
			if (Neighbours.size() != 0)
			{
				m_tileStack.push(currentTile);
				std::shared_ptr<MazeTile> tile = Neighbours[rand() % Neighbours.size()];
				int xmovement = (tile->XPos - currentTile->XPos) / 2;
				int ymovement = (tile->YPos - currentTile->YPos) / 2;


				if (xmovement != 0)
				{
					std::shared_ptr<MazeTile>  inBetweenTile = a_Maze[currentTile->XPos + xmovement][currentTile->YPos];
					inBetweenTile->Raised = false;
					wall = inBetweenTile->getWall();
					if (wall)
					{
						wall->hide();
					}
				}
				else if (ymovement != 0)
				{
					std::shared_ptr<MazeTile> inBetweenTile = a_Maze[currentTile->XPos][currentTile->YPos + ymovement];
					inBetweenTile->Raised = false;
					wall = inBetweenTile->getWall();
					if (wall)
					{
						wall->hide();
					}
				}

				currentTile = tile;

				currentTile->Raised = false;
				wall = currentTile->getWall();
				if (wall)
				{
					wall->hide();
				}
			}
			else if (m_tileStack.size() != 0)
			{
				currentTile = m_tileStack.top();
				m_tileStack.pop();
			}

		} while (m_tileStack.size() != 0);
	}

	std::vector<std::shared_ptr<MazeTile>> MazeGenerator::getNeighbours(std::vector<std::vector<std::shared_ptr<MazeTile>>> & a_Maze, MazeTile& a_Tile)
	{
		std::vector<std::shared_ptr<MazeTile>> neighbours;
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

	MazeGenerator::~MazeGenerator()
	{
	}
}
