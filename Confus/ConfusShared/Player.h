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

	/// <summary> Represents the state the player is currently in </summary>
	enum class EPlayerState : unsigned char
    {
		/// <summary> Represents the player as being alive</summary>
		Alive,
		/// <summary> Represents the player as executing a heavy attack</summary>
		HeavyAttacking,
		/// <summary> Represents the player as executing a light attack</summary>
		LightAttacking
    };

    /// <summary> 
    /// Player class
    /// </summary>
    class Player : public irr::scene::IAnimationEndCallBack, public irr::scene::ISceneNode
    {
    public:
		/// <summary>The state of the player carrying the flag</summary>
		ConfusShared::EFlagEnum CarryingFlag;
		/// <summary>The flag the player is currently holding</summary>
		ConfusShared::Flag* FlagPointer = nullptr;
		/// <summary>The damage to deal on a regular light attack</summary>
		static const unsigned LightAttackDamage;
		/// <summary>The damage to deal on a regular heavy attack</summary>
		static const unsigned HeavyAttackDamage;
		/// <summary>The event played once the player executes a light attack</summary>
		ConfusShared::Delegate<void()> OnLightAttack;
		/// <summary>The event played once the player executes a heavy attack</summary>
		ConfusShared::Delegate<void()> OnHeavyAttack;
    private:
		/// <summary>The m_ identifier</summary>
		long long m_ID;
		/// <summary> The IAnimatedMeshSceneNode for the player </summary>
		irr::scene::IAnimatedMeshSceneNode* m_PlayerNode;
		/// <summary> private value for health class </summary>
		ConfusShared::Health m_PlayerHealth;
		/// <summary>The collider of the player used for the collision detection</summary>
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
		/// <summary>The start position, which is where the player will respawn on death</summary>
		irr::core::vector3df m_StartPosition = irr::core::vector3df();
		/// <summary>The team identifier of the team the player belongs to</summary>
		ConfusShared::ETeamIdentifier m_TeamIdentifier;
		/// <summary>The direction in which the player is currently walking</summary>
		irr::core::vector3df m_WalkingDirection;
    public:
		/// <summary>Player class constructor.</summary>
		/// <param name="a_Device">The active Irrlicht Device.</param>
		/// <param name="a_PhysicsWorld">The Physics World</param>
		/// <param name="a_ID">The ID of the scenenode.</param>
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
		/// <summary>Gets the current state of the player, based on what it is currently doing</summary>
		/// <returns>The current player state</returns>
		EPlayerState getState() const;

		/// <summary> Changes the state of the player. </summary>
		void changeState(EPlayerState a_NewState);
		
		/// <summary>Sets the start position, which is where the player will be respawned once instructed to</summary>
		/// <param name="a_Position">The new respawn position</param>
		void setStartPosition(irr::core::vector3df a_Position);
		
		/// <summary>Gets the team identifier of the team the player belongs to</summary>
		/// <returns>The player's team identifier</returns>
		ConfusShared::ETeamIdentifier getTeamIdentifier() const;

		/// <summary>Sets the team identifier and color based on the new team identifier</summary>
		/// <param name="a_TeamIdentifier">The new team identifier</param>
		/// <param name="a_Device">The active Irrlicth device</param>
		void setTeamIdentifier(ConfusShared::ETeamIdentifier a_TeamIdentifier, irr::IrrlichtDevice* a_Device);
		
		/// <summary>Gets the current frame of the animation of the player</summary>
		/// <returns>The curren tanimation frame</returns>
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

		/// <summary>Sets the unique identifier that the player uses over the network</summary>
		/// <param name="a_ID">The new ID</param>
		void setGUID(long long a_ID);

		/// <summary>Gets the unique identifier that the player uses over the network</summary>
		long long Player::getGUID();
    private:
		/// <summary> Updates the color </summary>
		/// <param name="a_Device"> The active irrlicht device </param>
		void updateColor(irr::IrrlichtDevice* a_Device);
		
		/// <summary>Updates the velocity based on the walking direction</summary>
		void updateVelocity();

		/// <summary>Drops the flag the player is currently holding</summary>
		void dropFlag();
        /// <summary> Starts the walking animation, which is the default animation. </summary>
        void startWalking() const;
        /// <summary> Stops the walking animation. </summary>
        void stopWalking() const;
        /// <summary> Initializes the shared attack variables. </summary>
        void initializeAttack();

		/// <summary>Called when the animation finishes</summary>
		/// <param name="a_SceneNode">The scene node the animation ended on</param>
		/// <remarks>Generally used for the attack animations only</remarks>
		virtual void OnAnimationEnd(irr::scene::IAnimatedMeshSceneNode* a_SceneNode) override;
    };
}
