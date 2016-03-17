#pragma once
#include "Maze.h"
#include "DFSBacktracking.h"
namespace Confus
{
	class MazeGenerator
	{
	public:
		Maze Maze;
		DFSBacktracking DFSBackTracker;
	public:
		MazeGenerator(irr::IrrlichtDevice * a_Device, irr::core::vector3df a_StartPosition);
		void fixedUpdate();
		~MazeGenerator();
	};
}

