#pragma once
#include "MazeTile.h"

namespace ConfusShared
{
	namespace Physics
	{
		class PhysicsWorld;
	}

	/// <summary>
	/// Represents a tile used in mazes
	/// </summary>
	class WalledMazeTile : public MazeTile
	{
	private:
		/// <summary>
		/// The MoveableWall this tile contains
		/// </summary>
		MoveableWall m_Wall;
	public:
		/// <summary>
		/// Constructor that creates a moveableWall in a mazeTile
		/// </summary>
		/// <param name="a_Device">The current Irrlicht device.</param>
		/// <param name="a_RealPosition">The position of the wall when it is raised</param>
		/// <param name="a_Scale">The scale of the wall</param>
		/// <param name="a_PhysicsWorld">The active physics world</param>
		WalledMazeTile(irr::IrrlichtDevice* a_Device, irr::core::vector3df a_RealPosition, irr::core::vector3df a_Scale,
			Physics::PhysicsWorld& a_PhysicsWorld);

		/// <summary>
		/// The fixed update used to update the state of the walledMazeTile
		/// </summary>
		virtual void fixedUpdate() override;

		/// <summary>
		/// Getter for the MoveableWall, returns a pointer to the containing Wall.
		/// </summary>
		virtual MoveableWall* getWall() override;

		/// <summary>
		/// default destructor
		/// </summary>
		~WalledMazeTile();
	};
}

