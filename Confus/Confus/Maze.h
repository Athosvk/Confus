#pragma once
#include <vector>
#include "MazeTile.h"
namespace Confus
{
	class Maze
	{
	public:
		int MazeSizeX;
		int MazeSizeY;
		std::vector<std::vector<MazeTile>> MazeTiles;
	public:
		/// <summary>
		/// Constructor for this class
		/// </summary>
		/// <param name="a_Device">The current Irrlicht device.</param>
		/// <param name="a_StartPosition">Startposition is passed on in the constructor so we might be able to adjust the position where the maze is drawn</param>
		Maze(irr::IrrlichtDevice * a_Device, irr::core::vector3df a_StartPosition);
		void fixedUpdate();
		~Maze();
	};
}

