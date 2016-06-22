#pragma once
#include <vector>
#include <Irrlicht\irrlicht.h>

#include "NewMazeTile.h"

namespace ConfusShared
{

	/// <summary>
	/// Contains a 2D Vector of mazeTiles, forms the Maze
	/// </summary>
	class NewMaze
	{
	public:
		/// <summary>
		/// A 2D vector that contains all the MazeTiles
		/// </summary>
		std::vector<std::vector<std::shared_ptr<NewMazeTile>>> MazeTiles;

	private:
		/// <summary>
		/// the X size of the maze
		/// </summary>
		irr::core::vector2d<irr::u32> m_MazeSize;

	public:
		/// <summary>
		/// Gets the current size of the maze
		/// </summary>
		irr::core::vector2d<irr::u32> const& mazeSize() const;

		/// <summary> Constructor for this class </summary>
		/// <param name="a_MazeSizeX">The width </param>
		/// <param name="a_MazeSizeY">The depth </param>
		NewMaze(irr::core::vector2d<irr::u32> a_MazeSize);

		/// <summary>
		/// Resets the 2d vector
		/// </summary>
		void resetMaze();

		/// <summary>
		/// Empty destructor, default behaviour
		/// </summary>
		~NewMaze();
	};
}

