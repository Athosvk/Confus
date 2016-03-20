#include "MazeGenerator.h"
#include<iostream>

namespace Confus
{

	MazeGenerator::MazeGenerator(irr::IrrlichtDevice* a_Device, irr::core::vector3df a_StartPosition)
		: Maze(a_Device, a_StartPosition)
	{
		Maze.MazeTiles = DFSBackTracker.generateMaze(Maze.MazeTiles);
	}

	void MazeGenerator::fixedUpdate()
	{
		Maze.fixedUpdate();
	}


	MazeGenerator::~MazeGenerator()
	{
	}
}
