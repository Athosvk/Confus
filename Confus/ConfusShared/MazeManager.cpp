#include <RakNet\GetTime.h>

#include "MazeManager.h"


namespace ConfusShared
{
	MazeManager::MazeManager(irr::IrrlichtDevice * a_Device, irr::core::vector2d<irr::u32> a_MazeSize, int a_InitialSeed, Physics::PhysicsWorld& a_PhysicsWorld, float a_MazeScalar)
		:m_MainMaze(a_MazeSize), m_ReplacementMaze(a_MazeSize), m_Seed(a_InitialSeed),m_Device(a_Device), m_PhysicsWorld(a_PhysicsWorld), m_MazeScalar(a_MazeScalar)
	{
		m_MazeGenerator.generateMaze(m_MainMaze, a_InitialSeed, irr::core::vector2d<irr::u32>(20, 20));
		setupSingleMoveableWalls(m_MainMaze, irr::core::vector2df(30, -7));
	}

	void MazeManager::fixedUpdate()
	{
		int currentTime = RakNet::GetTimeMS();
		if (!hasBeenRefilled && currentTime > refillMazeTime)
		{
			refillMainMaze(m_Seed);
			hasBeenRefilled = true;
		}
		
		for (auto wallVector : MoveableWalls)
		{
			for (auto wall : wallVector)
			{
				wall->fixedUpdate();
			}
		}
	}

	void MazeManager::addMazeChangedListener(std::function<void()> a_Callback)
	{
		m_OnMazeChange += a_Callback;
	}

	void MazeManager::refillMainMaze(int a_Seed)
	{
		m_MazeGenerator.generateMaze(m_ReplacementMaze, a_Seed, irr::core::vector2d<irr::u32>(20, 20));
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
						MoveableWalls[x][y]->rise();
					}
				}
				else
				{
					if (m_MainMaze.MazeTiles[x][y]->Raised)
					{
						m_MainMaze.MazeTiles[x][y]->Raised = false;
						MoveableWalls[x][y]->hide();
					}
				}
			}
		}
	}

	void MazeManager::setupSingleMoveableWalls(NewMaze & a_Maze, irr::core::vector2df a_Offset)
	{
		//MoveableWall(irr::IrrlichtDevice* a_Device, irr::core::vector3df a_RegularPosition,
		//	irr::core::vector3df a_Scale, Physics::PhysicsWorld& a_PhysicsWorld);
		MoveableWalls.clear();
		for (int x = 0; x < static_cast<int>(a_Maze.mazeSize().X); x++)
		{
			std::vector<std::shared_ptr<MoveableWall>> tempVector;
			MoveableWalls.push_back(tempVector);
			for (int y = 0; y < static_cast<int>(a_Maze.mazeSize().Y) - 1; y++)
			{
				std::shared_ptr<MoveableWall> wall = std::make_shared<MoveableWall>(m_Device,
					irr::core::vector3df(static_cast<float>(-x*m_MazeScalar + a_Offset.X), 0.5f, static_cast<float>(-y*m_MazeScalar + a_Offset.Y)),
					irr::core::vector3df(m_MazeScalar, 1., m_MazeScalar), //do not want to raise the wall height
					m_PhysicsWorld);
				irr::scene::IAnimatedMeshSceneNode* wallMeshNode = wall->getMeshNode();
				irr::core::vector3df boundingBox = wallMeshNode->getBoundingBox().getExtent();
				wall->HiddenPosition = irr::core::vector3df(static_cast<float>(-x * m_MazeScalar + a_Offset.X),
					-boundingBox.Y * wallMeshNode->getScale().Y, static_cast<float>(-y*m_MazeScalar + a_Offset.Y));
				wall->TransitionSpeed = 0.01f;
				MoveableWalls[x].push_back(wall);
				if (!m_MainMaze.MazeTiles[x][y]->Raised)
				{
					MoveableWalls[x][y]->hide();
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
