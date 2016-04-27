#pragma once
#include "MazeTile.h"

namespace Confus
{
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
		WalledMazeTile(irr::IrrlichtDevice* a_Device, irr::core::vector3df a_RealPosition);

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

