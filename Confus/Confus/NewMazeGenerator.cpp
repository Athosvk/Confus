#include <RakNet\GetTime.h>

#include "NewMazeGenerator.h"
#include "WalledMazeTile.h"
#include "Audio/AudioManager.h"

namespace Confus
{

	NewMazeGenerator::NewMazeGenerator(NewMaze a_Maze, int a_InitialSeed, irr::core::vector2df a_GenerateStartPoint)
		: m_MainMaze(a_Device, a_MazeSizeX, a_MazeSizeY, a_PhysicsWorld, 1.5f, true),
		m_ReplacementMaze(a_Device, a_MazeSizeX, a_MazeSizeY, a_PhysicsWorld, 1.5f),
		m_Seed(a_InitialSeed), m_GenerateStartPoint(a_GenerateStartPoint),
		m_MazeChangeSound(a_AudioManager->createSound("Wall rising.wav"))
	{
		generateMaze(m_MainMaze.MazeTiles, a_InitialSeed);
		m_MazeChangeSound.setVolume(0.2f);
	}

	void NewMazeGenerator::fixedUpdate()
	{
		int currentTime = RakNet::GetTimeMS();
		if (!hasBeenRefilled && currentTime > refillMazeTime)
		{
			refillMainMaze(m_Seed);
			hasBeenRefilled = true;
		}
		m_MainMaze.fixedUpdate();
	}

	void NewMazeGenerator::refillMainMaze(int a_Seed)
	{
		m_MazeChangeSound.play();
		generateMaze(m_ReplacementMaze.MazeTiles, a_Seed);
		replaceMainMaze();
		m_ReplacementMaze.resetMaze(irr::core::vector2df(30, -7), false);
	}

	void NewMazeGenerator::replaceMainMaze()
	{
		for (int x = 0; x < m_MainMaze.mazeSizeX(); x++)
		{
			for (int y = 0; y < m_MainMaze.mazeSizeY() - 1; y++)
			{
				if (m_ReplacementMaze.MazeTiles[x][y]->Raised)
				{
					if (!m_MainMaze.MazeTiles[x][y]->Raised)
					{
						m_MainMaze.MazeTiles[x][y]->Raised = true;
						m_MainMaze.MazeTiles[x][y]->getWall()->rise();
					}
				}
				else
				{
					if (m_MainMaze.MazeTiles[x][y]->Raised)
					{
						m_MainMaze.MazeTiles[x][y]->Raised = false;
						m_MainMaze.MazeTiles[x][y]->getWall()->hide();
					}
				}
			}
		}
	}

	void NewMazeGenerator::generateMaze(std::vector<std::vector<std::shared_ptr<MazeTile>>> &  a_Maze, int a_Seed)
	{
		//setup globals & variables
		srand(a_Seed);
		MoveableWall* wall = nullptr;
		std::shared_ptr<MazeTile> currentTile = a_Maze[static_cast<int>(m_GenerateStartPoint.X)][static_cast<int>(m_GenerateStartPoint.Y)];

		//Startcell must be set to visited, add to visitedcount
		currentTile->Raised = false;

		wall = currentTile->getWall();
		if (wall)
		{
			wall->hide();
		}

		do
		{
			std::vector<std::shared_ptr<MazeTile>> Neighbours = getNeighbours(a_Maze, *currentTile);
			if (Neighbours.size() != 0)
			{
				m_TileStack.push(currentTile);
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
			else if (m_TileStack.size() != 0)
			{
				currentTile = m_TileStack.top();
				m_TileStack.pop();
			}

		} while (m_TileStack.size() != 0);
	}

	std::vector<std::shared_ptr<MazeTile>> NewMazeGenerator::getNeighbours(std::vector<std::vector<std::shared_ptr<MazeTile>>> & a_Maze, MazeTile& a_Tile)
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

	void NewMazeGenerator::refillMainMazeRequest(int a_Seed, int a_ChangeWantedTime)
	{
		if (a_ChangeWantedTime > refillMazeTime)
		{
			refillMazeTime = a_ChangeWantedTime;
			m_Seed = a_Seed;
			hasBeenRefilled = false;
		}
	}

	NewMazeGenerator::~NewMazeGenerator()
	{
	}
}
