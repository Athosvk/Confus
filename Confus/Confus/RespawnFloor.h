#pragma once
#include <Irrlicht/irrlicht.h>

namespace Confus 
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
        /// <summary> The IAnimatedMeshSceneNode for the respawn floor. </summary>
        irr::scene::IAnimatedMeshSceneNode* m_FloorNode;
        /// <summary> The Solid texture for the respawn floor. </summary>
        irr::video::ITexture* m_RegularTexture;
        /// <summary> The Transparent texture for the respawn floor. </summary>
        irr::video::ITexture* m_TransparentTexture;
		Physics::RigidBody* m_RigidBody = nullptr;
    public:
		/// <summary> The constructor for the respawn floor </summary>
		/// <param name="a_Device">The active Irrlicht Device.</param>
		/// <param name="a_PhysicsWorld">The physics world</param>
		/// <param name="a_Position">The position</param>
		RespawnFloor(irr::IrrlichtDevice* a_Device, Physics::PhysicsWorld& a_PhysicsWorld, 
			irr::core::vector3df a_Position);
        /// <summary> The destructor for the respawn floor. </summary>
        ~RespawnFloor();
        /// <summary> Enables the collision of the respawn floor and sets its texture to solid. </summary>
        void enableCollision();
        /// <summary> Disables the collision of the respawn floor and sets its texture to transparent. </summary>
        void disableCollision();
    };
}


