#pragma once
#include <vector>

#include "MazeTile.h"

namespace ConfusShared
{
	namespace Physics
	{
		class PhysicsWorld;
	}

	/// <summary>
	/// Contains a 2D Vector of mazeTiles, forms the Maze
	/// </summary>
	class Maze
	{
	public:
		/// <summary>
		/// A 2D vector that contains all the MazeTiles
		/// </summary>
		std::vector<std::vector<std::shared_ptr<MazeTile>>> MazeTiles;

	private:
		/// <summary>
		/// The current IrrlichtDevice. Needed to pass to created MazeTiles.
		/// </summary>
		irr::IrrlichtDevice* m_IrrDevice;
		
		/// <summary>The phsyics world to place the colliders for the maze in</summary>
		Physics::PhysicsWorld& m_PhysicsWorld;

		/// <summary>
		/// the X size of the maze
		/// </summary>
		int m_MazeSizeX;

		/// <summary>
		/// The Y size of the maze
		/// </summary>
		int m_MazeSizeY;

		/// <summary>
		/// This Scalar increases the scale of the walls and the position that that walls are from each other thus making larger paths.
		/// </summary>
		float m_MazeScalar;

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
		/// <param name="a_Device">The current Irrlicht device.</param>
		/// <param name="a_MazeSizeX">The width </param>
		/// <param name="a_MazeSizeY">The depth </param>
		/// <param name="a_PhysicsWorld">The active physics world.</param>
		/// <param name="a_MazeScalar">The scale of the maze</param>
		/// <param name="a_NeedRender">Boolean that states if this maze needs to be rendered or not</param>
		Maze(irr::IrrlichtDevice * a_Device, int a_MazeSizeX, int a_MazeSizeY, Physics::PhysicsWorld& a_PhysicsWorld,
			float a_MazeScalar = 1, bool a_NeedRender = false);

		/// <summary>
		/// Resets the 2d vector, raising all mazetiles in it or making it a rendered maze
		/// </summary>
		/// <param name="a_Offset">The offset used to position the maze from the starting position.</param>
		/// <param name="a_NeedRender">Boolean that states if this maze needs to be rendered or not</param>
		void resetMaze(irr::core::vector2df a_Offset, bool a_NeedRender = false);

		/// <summary>
		/// The fixed update used to update the state of the maze
		/// </summary>
		void fixedUpdate();

		/// <summary>
		/// Empty destructor, default behaviour
		/// </summary>
		~Maze();
	};
}

