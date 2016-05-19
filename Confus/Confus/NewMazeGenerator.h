#pragma once
#include <stack>
#include "NewMaze.h"

namespace Confus
{
	/// <summary>
	/// Generates the maze
	/// </summary>
	class NewMazeGenerator
	{
	private:
		/// <summary>
		/// This stack is used by the maze generating algorithm to keep track of mazeTiles that might still have neighbours that can be accesed
		/// </summary>
		std::stack<std::shared_ptr<NewMazeTile>> m_TileStack;
	public:
		/// <summary> Initializes a new instance of the <see cref="MazeGenerator" /> class </summary>
		NewMazeGenerator();

		/// <summary>
		/// Using a depth first search algorithm generates a maze for the given 2d vector maze with a seed
		/// </summary>
		/// <param name="a_Maze">The 2d maze that is going to be used in the generation.</param>
		/// <param name="a_Seed">The seed that is going to be used in the generation.</param>
		/// <param name="a_GenerateStartPoint">The start position used to start generating the maze.</param>
		void generateMaze(NewMaze &  a_Maze, int a_Seed, irr::core::vector2df a_GenerateStartPoint);

		~NewMazeGenerator();
	private:
		/// <summary>
		/// Loads the necessary textures
		/// </summary>
		/// <param name="a_Maze">The 2d vector to be used to check for neighbours.</param>
		/// <param name="a_Tile">The tile in the 2d vector that is checked for neighbours.</param>
		std::vector<std::shared_ptr<NewMazeTile>> getNeighbours(std::vector<std::vector<std::shared_ptr<NewMazeTile>>> & a_Maze, NewMazeTile& a_Tile);
	};
}

