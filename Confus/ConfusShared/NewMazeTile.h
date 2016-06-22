#pragma once
#include <string>
#include <memory>

namespace ConfusShared
{
	/// <summary>
	/// Represents a tile used in mazes
	/// </summary>
	class NewMazeTile
	{
	public:
		/// <summary>
		/// If the tile is raised or lowered
		/// </summary>
		bool Raised = true;
		bool Wall = false;

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
		NewMazeTile();

		bool IsAvailable();

		/// <summary>
		/// Default destructor
		/// </summary>
		~NewMazeTile();
	};
}

