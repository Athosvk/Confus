#pragma once
#include <memory>

#include "Collider.h"

namespace Confus
{
    class Weapon
    {
    private:
        std::unique_ptr<Collider> m_Collider = nullptr;
        irr::scene::ITriangleSelector* m_TriangleSelector;
        irr::scene::ISceneNode* m_Node;

    public:
        Weapon(irr::scene::ISceneNode* a_ParentNode, irr::scene::ISceneManager* a_SceneManager);
        
        /// <summary>
        /// Enables the collider.of the weapon, making it possible to damage players
        /// </summary>
        /// <remarks> 
        /// This is to allow the enclosing class to play attack animations, so that it
        /// cannot hit other entities when not supposed to.
        /// </remarks>
        void enableCollider();
        /// <summary>
        /// Disables the collider of the weapon, preventing it from damaging players
        /// </summary>
        /// <remarks> <see cref="enableCollider"> </remarks>
        void disableCollider();
    private:
        /// <summary>
        /// Damages the player.
        /// </summary>
        /// <remarks> Triggered as callback on collision </remarks>
        void damagePlayer() const;
    };
}
