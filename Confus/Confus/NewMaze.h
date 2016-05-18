#pragma once
#include <vector>

#include "NewMazeTile.h"

namespace Confus
{
	namespace Physics
	{
		class PhysicsWorld;
	}

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
		int m_MazeSizeX;

		/// <summary>
		/// The Y size of the maze
		/// </summary>
		int m_MazeSizeY;

	public:
		/// <summary>
		/// Gets the current X size of the maze
		/// </summary>
		int const& mazeSizeX() const;

		/// <summary>
		/// Gets the current Y size of the maze
		/// </summary>
		int const& mazeSizeY() const;

		/// <summary> Constructor for this class </summary>
		/// <param name="a_MazeSizeX">The width </param>
		/// <param name="a_MazeSizeY">The depth </param>
		NewMaze(int a_MazeSizeX, int a_MazeSizeY);

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

