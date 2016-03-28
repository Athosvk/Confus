#pragma once
#include <stack>

#include "Maze.h"
namespace Confus
{
	class MazeGenerator
	{
	public:
		int Seed;
		Maze MainMaze;
		Maze ReplacementMaze;

		std::stack<MazeTile, std::vector<std::shared_ptr<MazeTile>>> TileStack;
	public:
		MazeGenerator(irr::IrrlichtDevice * a_Device, irr::core::vector3df a_StartPosition, int a_InitialSeed);
		void fixedUpdate();
		void refillMainMaze(int a_Seed);
		~MazeGenerator();
	private:
		std::vector<std::shared_ptr<MazeTile>> getNeighbours(std::vector<std::vector<std::shared_ptr<MazeTile>>> & a_Maze, MazeTile& a_Tile);
		void generateMaze(std::vector<std::vector<std::shared_ptr<MazeTile>>> &  a_Maze, int a_Seed);
	};
}

