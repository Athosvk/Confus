#pragma once
#include "../ConfusShared/Physics/BoxCollider.h"

namespace Confus
{
	class Player;
}

namespace ConfusShared
{
	namespace Physics
	{
		class PhysicsWorld;
	}
    /// <summary> The weapon used by the player in order to deal damage to others </summary>
    /// <remarks> 
    /// Will basically be used by the Player class only,
    /// since it the only one with the ability to deal damage as of now
    /// </remarks>
    class Weapon
    {
    public:
        /// <summary> The damage dealt by the weapon on hit </summary>
        unsigned Damage = 0;

        /// <summary> The angle at which an attack is seen as a backstab</summary>
        irr::f32 BackstabAngle = 45.0f;
    private:
        /// <summary> The collider </summary>
		Physics::BoxCollider* m_Collider;

        /// <summary> The irrlicht scene node </summary>
        irr::scene::ISceneNode* m_Node;

        /// <summary> Whether a collision has been trigered between now and the call to <see cref="resetCollider"> </summary> 
        /// <remarks> Used to prevent the weapon from dealing damage multiple times to the same collided node </remarks>
        bool m_Collided;

    public:        
		/// <summary> Initializes a new instance of the <see cref="Weapon" /> class </summary>
		/// <param name="a_SceneManager">The active scene manager</param>
		/// <param name="a_World">The physics world </param>
		/// <param name="a_Dimensions">The dimensions of the weapon</param>
		Weapon(irr::scene::ISceneManager* a_SceneManager, Physics::PhysicsWorld& a_World,
			irr::core::vector3df a_Dimensions);

        /// <summary> Enables the collider.of the weapon, making it possible to damage players </summary>
        /// <remarks> 
        /// This is to allow the enclosing class to play attack animations, so that it
        /// cannot hit other entities when not supposed to.
        /// </remarks>
        void enableCollider();

        /// <summary> Disables the collider of the weapon, preventing it from damaging players </summary>
        /// <remarks> 
		/// This is to allow the enclosing class to play attack animations, so that it
        /// cannot hit other entities when not supposed to. 
		/// </remarks>
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
        void damagePlayer(Confus::Player* a_Player) const;

        /// <summary> Gets the angle between the given vectors </summary>
        /// <param name="a_Vector1">The first vector </param>
        /// <param name="a_Vector2">The other vector to compare the angle with </param>
        /// <returns> The angle </returns>
        float getAngle(irr::core::vector3df a_Vector1, irr::core::vector3df a_Vector2) const;
    };
}
