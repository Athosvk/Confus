#pragma once
#include "NewMaze.h"
#include "NewMazeGenerator.h"
namespace Confus
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

		/// <summary>
		/// Generator to generate mazes
		/// </summary>
		NewMazeGenerator m_MazeGenerator;

		/// <summary>
		/// The time at which the generator will refill the maze again
		/// <summary>
		int refillMazeTime;

		/// <summary>
		/// A bool that checks if the maze fill request has been fulfilled yet
		/// </summary>
		bool hasBeenRefilled = true;

	public:
		MazeManager(irr::core::vector2d<irr::u32> a_MazeSize);
		void fixedUpdate();
		void refillMainMaze(int a_Seed);
		void replaceMainMaze();
		void refillMainMazeRequest(int a_Seed, int a_ChangeWantedTime);
		~MazeManager();
	};
}

