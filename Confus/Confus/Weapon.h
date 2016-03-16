#pragma once
#include <memory>

#include "Collider.h"

namespace Confus
{
    class Weapon
    {
    private:        
        /// <summary> The collider instance used for collision callbacks </summary>
        std::unique_ptr<Collider> m_Collider = nullptr;

        /// <summary> The triangle selector used for collision </summary>
        /// <remarks> Stored so that we can temporarily disable it </remarks>
        irr::scene::ITriangleSelector* m_TriangleSelector;

        /// <summary> The irrlicht scnee node </summary>
        irr::scene::ISceneNode* m_Node;

        /// <summary> Whether a collision has been trigered between now and the call to <see cref="resetCollider"> </summary> 
        /// <remarks> Used to prevent the weapon from dealing damage multiple times to the same collided node </remarks>
        bool m_Collided;

    public:
        Weapon(irr::scene::ISceneManager* a_SceneManager, irr::core::vector3df a_Dimensions);

        /// <summary> Enables the collider.of the weapon, making it possible to damage players </summary>
        /// <remarks> 
        /// This is to allow the enclosing class to play attack animations, so that it
        /// cannot hit other entities when not supposed to.
        /// </remarks>
        void enableCollider();

        /// <summary> Disables the collider of the weapon, preventing it from damaging players </summary>
        /// <remarks> <see cref="enableCollider"> </remarks>
        void disableCollider();       

        /// <summary>
        /// Resets the collider so that collisions can re-occur
        /// </summary>
        /// <remarks> Used so prevent from dealing damage twice during the same attack </remarks>
        void resetCollider();

        /// <summary> Sets the parent transformation node </summary>
        /// <remarks> Used to bind the weapon to a joint, so that it can be used for animations </remarks>
        /// <param name="a_Parent">The new parent </param>
        void setParent(irr::scene::ISceneNode* a_Parent);

    private:
        /// <summary> Damages the playe. </summary>
        /// <remarks> Triggered as callback on collision </remarks>
        void damagePlayer() const;
    };
}
