#pragma once
#include <Irrlicht/irrlicht.h>

namespace ConfusShared
{
	namespace Physics
	{
		class PhysicsWorld;
		class RigidBody;
	}
    /// <summary> 
    /// Floor of the respawn chambers located in each base,
    /// has functions to become transparent/solid and disable/enable
    /// collision so players can fall through and respawn.
    /// </summary>
    class RespawnFloor
    {
    private:
		static const float PassThroughTime;

        /// <summary> The IAnimatedMeshSceneNode for the respawn floor. </summary>
        irr::scene::IAnimatedMeshSceneNode* m_FloorNode;
        /// <summary> The Solid texture for the respawn floor. </summary>
        irr::video::ITexture* m_RegularTexture;
        /// <summary> The Transparent texture for the respawn floor. </summary>
        irr::video::ITexture* m_TransparentTexture;
		/// <summary>The rigid body to handle collision for this respawn floor</summary>
		Physics::RigidBody* m_RigidBody = nullptr;		
		/// <summary>The timer keeping track of the time before solidifying the floor again</summary>
		float m_PassThroughTimer = 0.0f;

    public:
		/// <summary> The constructor for the respawn floor </summary>
		/// <param name="a_Device">The active Irrlicht Device.</param>
		/// <param name="a_PhysicsWorld">The physics world</param>
		/// <param name="a_Position">The position</param>
		RespawnFloor(irr::IrrlichtDevice* a_Device, Physics::PhysicsWorld& a_PhysicsWorld, 
			irr::core::vector3df a_Position);
		
		/// <summary>Updates the specified timer of the respawnfloor if active</summary>
		/// <param name="a_DeltaTime">The time since the last update</param>
		void update(float a_DeltaTime);

		/// <summary> Disables the collision of the respawn floor and sets its texture to transparent. </summary>
		void disableCollision();
	private:
		/// <summary> Enables the collision of the respawn floor and sets its texture to solid. </summary>
		void enableCollision();
    };
}


