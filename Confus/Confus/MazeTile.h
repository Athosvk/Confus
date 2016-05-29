#pragma once
#include <string>
#include <memory>

#include "MoveableWall.h"

namespace Confus
{
	/// <summary>
	/// Represents a tile used in mazes
	/// </summary>
	class MazeTile
	{
	public:
		/// <summary>
		/// If the tile is raised or lowered
		/// </summary>
		bool Raised = true;

		/// <summary>
		/// The X position of this tile
		/// </summary>
		int XPos = 0;

		/// <summary>
		/// The Y position of this tile
		/// </summary>
		int YPos = 0;
	public:
		/// <summary>
		/// Default constructor
		/// </summary>
		MazeTile();

		/// <summary>
		/// Virtual method, does nothing.
		/// </summary>
		virtual void fixedUpdate();

		/// <summary>
		/// Virtual method, returns nullptr.
		/// </summary>
		virtual MoveableWall* getWall();

		/// <summary>
		/// Default destructor
		/// </summary>
		~MazeTile();
	};
}

