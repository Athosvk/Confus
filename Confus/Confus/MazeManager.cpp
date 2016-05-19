#include <RakNet\GetTime.h>

#include "MazeManager.h"


namespace Confus
{
	MazeManager::MazeManager(irr::core::vector2d<irr::u32> a_MazeSize)
		:m_MainMaze(a_MazeSize), m_ReplacementMaze(a_MazeSize)
	{
	}

	void MazeManager::fixedUpdate()
	{
		int currentTime = RakNet::GetTimeMS();
		if (!hasBeenRefilled && currentTime > refillMazeTime)
		{
			refillMainMaze(m_Seed);
			hasBeenRefilled = true;
		}
		
		//for (auto wallVector : MazeTiles)
		//{
		//	for (auto wall : wallVector)
		//	{
		//		wall->fixedUpdate();
		//	}
		//}
	}

	void MazeManager::refillMainMaze(int a_Seed)
	{
		m_MazeGenerator.generateMaze(m_ReplacementMaze, a_Seed, irr::core::vector2df(0,0));
		replaceMainMaze();
		m_ReplacementMaze.resetMaze();
	}

	void MazeManager::replaceMainMaze()
	{
		for (unsigned int x = 0; x < m_MainMaze.mazeSize().X; x++)
		{
			for (unsigned int y = 0; y < m_MainMaze.mazeSize().Y - 1; y++)
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

	void MazeManager::refillMainMazeRequest(int a_Seed, int a_ChangeWantedTime)
	{
		if (a_ChangeWantedTime > refillMazeTime)
		{
			refillMazeTime = a_ChangeWantedTime;
			m_Seed = a_Seed;
			hasBeenRefilled = false;
		}
	}


	MazeManager::~MazeManager()
	{
	}
}
