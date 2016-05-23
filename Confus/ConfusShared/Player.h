#pragma once
#include <irrlicht/irrlicht.h>

#include "Health.h"
#include "Weapon.h"
#include "TeamIdentifier.h"
#include "Delegate.h"

namespace ConfusShared
{
	class EventManager;
	class Flag;
	enum class EFlagEnum;
	namespace Physics
	{
		class BoxCollider;
		class PhysicsWorld;
	}

    enum class EPlayerState : unsigned char
    {
        Alive, CarryingFlag, HeavyAttacking, LightAttacking, Dead
    };

    /// <summary> 
    /// Player class
    /// </summary>
    class Player : public irr::scene::IAnimationEndCallBack, public irr::scene::ISceneNode
    {
    public:
		ConfusShared::EFlagEnum CarryingFlag;
        ConfusShared::Flag* FlagPointer = nullptr;

		static const unsigned LightAttackDamage;
		static const unsigned HeavyAttackDamage;

		ConfusShared::Delegate<void()> OnLightAttack;
		ConfusShared::Delegate<void()> OnHeavyAttack;
    private:
		/// <summary>The m_ identifier</summary>
		long long m_ID;
		/// <summary> The IAnimatedMeshSceneNode for the player </summary>
		irr::scene::IAnimatedMeshSceneNode* m_PlayerNode;
		/// <summary> private value for health class </summary>
		ConfusShared::Health m_PlayerHealth;
		ConfusShared::Physics::BoxCollider* m_Collider;

        /// <summary> The weapon bone index of the animation for the weapon </summary>
        static const irr::u32 WeaponJointIndex;

        /// <summary> The player's weapon </summary>
        ConfusShared::Weapon m_Weapon;

        /// <summary> Whether the player is currently attacking or not </summary>
        bool m_Attacking = false;

        /// <summary> Whether the player is currently walking or not </summary>
        bool m_Walking = false;

        /// <summary> The player's mesh </summary>
        irr::scene::IAnimatedMesh* m_Mesh;
        /// <summary> The player's active state. </summary>
        EPlayerState m_PlayerState = EPlayerState::Alive;
		irr::core::vector3df m_StartPosition = irr::core::vector3df();

		ConfusShared::ETeamIdentifier m_TeamIdentifier;

		irr::core::vector3df m_WalkingDirection;
    public:
		/// <summary>Player class constructor.</summary>
		/// <param name="a_Device">The active Irrlicht Device.</param>
		/// <param name="a_PhysicsWorld">The Physics World</param>
		/// <param name="a_ID">The ID of the scenenode.</param>
		/// <param name="a_TeamIdentifier">The team's identifier the player should have.</param>
		Player(irr::IrrlichtDevice* a_Device, ConfusShared::Physics::PhysicsWorld& a_PhysicsWorld, long long a_ID);
        /// <summary> Update function, any tasks that need to be done every frame go here. </summary>
        void update();
        ///<summary> Respawns the player to their base, public so round resets etc. can call this. </summary>
        void respawn();

        /// <summary> Required render function for the ISceneNode, does nothing as we render in the Game.cpp.</summary>
        virtual void render();
        /// <summary> Returns the bounding box of the player's mesh. </summary>
        virtual const irr::core::aabbox3d<irr::f32> & getBoundingBox() const;

		/// <summary> get/setter for health. Should really turn into const but gave some weird errors </summary>
		ConfusShared::Health* getHealthInstance();
		/// <summary> Starts the light attack, dealing normal damage. </summary>
		void startLightAttack();

		/// <summary> Starts the heavy attack, which deals more damage. </summary>
		void startHeavyAttack();
		EPlayerState getState() const;
		void setStartPosition(irr::core::vector3df a_Position);
		ConfusShared::ETeamIdentifier getTeamIdentifier() const;
		void setTeamIdentifier(ConfusShared::ETeamIdentifier a_TeamIdentifier, irr::IrrlichtDevice* a_Device);
		int getAnimationFrame() const;		
		/// <summary>
		/// Sets the movement direction, so that the player can walk in other directions than forward,
		/// and will update the velocity accordingly 
		/// </summary>
		/// <param name="a_Direction">The direction the player will start moving in in local terms</param>
		void setWalkingDirection(irr::core::vector3df a_Direction);

		/// <summary>Gets the unique identifier.</summary>
		/// <returns>The unique identifier of this player instance</returns>
		long long getGUID() const;

		void setGUID(long long a_ID);
    private:
		/// <summary> Updates the color </summary>
		/// <param name="a_Device"> The active irrlicht device </param>
		void updateColor(irr::IrrlichtDevice* a_Device);
		
		/// <summary>Updates the velocity based on the walking direction</summary>
		void updateVelocity();

		void dropFlag();
        /// <summary> Starts the walking animation, which is the default animation. </summary>
        void startWalking() const;
        /// <summary> Stops the walking animation. </summary>
        void stopWalking() const;
        /// <summary> Initializes the shared attack variables. </summary>
        void initializeAttack();
        
        /// <summary> Changes the state of the player. </summary>
        void changeState(EPlayerState a_NewState);

		/// <summary>Called when the animation finishes</summary>
		/// <param name="a_SceneNode">The scene node the animation ended on</param>
		/// <remarks>Generally used for the attack animations only</remarks>
		virtual void OnAnimationEnd(irr::scene::IAnimatedMeshSceneNode* a_SceneNode) override;
    };
}
