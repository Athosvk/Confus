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
		int Identifier;
		std::vector<std::vector<std::shared_ptr<MazeTile>>> MazeTiles;
	private:
		/// <summary>
		/// The current IrrlichtDevice. Needed to pass to created MazeTiles.
		/// </summary>
		irr::IrrlichtDevice* IrrDevice;
	public:
		/// <summary>
		/// Constructor for this class
		/// </summary>
		/// <param name="a_Device">The current Irrlicht device.</param>
		/// <param name="a_StartPosition">Startposition is passed on in the constructor so we might be able to adjust the position where the maze is drawn</param>
		Maze(irr::IrrlichtDevice * a_Device, irr::core::vector3df a_StartPosition, bool a_NeedRender);
		void resetMaze(irr::core::vector2df a_Offset, bool a_NeedRender);
		void fixedUpdate();
		~Maze();
	};
}

