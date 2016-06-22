#pragma once
#include <vector>

#include "NewMaze.h"
#include "NewMazeGenerator2.h"
#include "MoveableWall.h"
#include "Delegate.h"
namespace ConfusShared
{
	class MazeManager
	{
	private:

		/// <summary>
		/// The maze that the players walk in.
		/// </summary>
		NewMaze m_MainMaze;

		/// <summary>
		/// The maze that is used to generate a new maze. The main maze is steadely replaced by this one.
		/// Does not render walls.
		/// </summary>
		NewMaze m_ReplacementMaze;

		/// <summary>
		/// The seed used to randomly chose an available neighbour and thus the seed that determines the layout of the maze.
		/// </summary>
		int m_Seed;

		irr::IrrlichtDevice * m_Device;
		Physics::PhysicsWorld& m_PhysicsWorld;

		/// <summary>
		/// This Scalar increases the scale of the walls and the position that that walls are from each other thus making larger paths.
		/// </summary>
		float m_MazeScalar;

		/// <summary>
		/// Generator to generate mazes
		/// </summary>
		NewMazeGenerator2 m_MazeGenerator;

		/// <summary>
		/// The time at which the generator will refill the maze again
		/// <summary>
		int refillMazeTime;

		/// <summary>
		/// A bool that checks if the maze fill request has been fulfilled yet
		/// </summary>
		bool hasBeenRefilled = true;

		/// <summary>
		/// A 2D vector that contains all the MoveableWalls
		/// </summary>
		std::vector<std::vector<std::shared_ptr<MoveableWall>>> MoveableWalls;

		/// <summary> 
		/// Called once the maze changes, so that we can perform actions such as send the new maze over the network
		/// or play the associated sounds
		/// </summary>
		Delegate<void()> m_OnMazeChange;
		

	public:
		MazeManager(irr::IrrlichtDevice * a_Device, irr::core::vector2d<irr::u32> a_MazeSize, int a_InitialSeed, Physics::PhysicsWorld& a_PhysicsWorld, float a_MazeScalar);
		void fixedUpdate();
		void addMazeChangedListener(std::function<void()> a_Callback);
		void refillMainMaze(int a_Seed);
		void replaceMainMaze();
		void setupSingleMoveableWalls(NewMaze & a_Maze, irr::core::vector2df a_Offset);
		void refillMainMazeRequest(int a_Seed, int a_ChangeWantedTime);
		~MazeManager();
	};
}

