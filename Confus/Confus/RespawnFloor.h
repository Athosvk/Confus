#pragma once

#include <irrlicht/irrlicht.h>

namespace Confus 
{
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
        /// <summary> The ITriangleSelector for the respawn floor (used for collisions). </summary>
        irr::scene::ITriangleSelector* m_TriangleSelector;
        /// <summary> The Solid texture for the respawn floor. </summary>
        irr::video::ITexture* m_RegularTexture;
        /// <summary> The Transparent texture for the respawn floor. </summary>
        irr::video::ITexture* m_TransparentTexture;
    public:
        /// <summary> The constructor for the respawn floor. </summary>
        /// <param name="a_Device">The active Irrlicht Device.</param>
        RespawnFloor(irr::IrrlichtDevice* a_Device);
        /// <summary> The destructor for the respawn floor. </summary>
        ~RespawnFloor();
        /// <summary> This function sets the position of the respawn floor. </summary>
        /// <param name="a_NewPosition">The new position for the respawn floor.</param>
        void setPosition(irr::core::vector3df a_NewPosition);
        /// <summary> Enables the collision of the respawn floor and sets its texture to solid. </summary>
        void enableCollision();
        /// <summary> Disables the collision of the respawn floor and sets its texture to transparent. </summary>
        void disableCollision();
    };
}


