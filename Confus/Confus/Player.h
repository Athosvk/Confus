#pragma once
#include <irrlicht/irrlicht.h>

#include "Networking\ClientConnection.h"
#include "../ConfusShared/Health.h"
#include "../ConfusShared/Weapon.h"
#include "../ConfusShared/TeamIdentifier.h"

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
}

namespace Confus 
{
    namespace Audio
    {
        class PlayerAudioEmitter;
        class AudioManager;
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
		/// <summary> The IAnimatedMeshSceneNode for the player </summary>
        irr::scene::IAnimatedMeshSceneNode* PlayerNode;
        /// <summary> The ICameraSceneNode for the player </summary>
        irr::scene::ICameraSceneNode* CameraNode = nullptr;

		ConfusShared::EFlagEnum CarryingFlag;
		ConfusShared::ETeamIdentifier TeamIdentifier;    
        ConfusShared::Flag* FlagPointer = nullptr;
        /// <summary> Determines if this player is this users player or not </summary>
        bool MainPlayer = false;

        long long ID;

		static const unsigned LightAttackDamage;
		static const unsigned HeavyAttackDamage;
    private:
        Audio::PlayerAudioEmitter* m_SoundEmitter;
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
        /// <summary> The player's unique ID. </summary>
        RakNet::RakNetGUID m_PlayerID;
        /// <summary> The player's active state. </summary>
        EPlayerState m_PlayerState = EPlayerState::Alive;
        /// <summary> The local time at which a state change took place. </summary>
        RakNet::Time m_StateChangeTime = 0;
    public:
        /// <summary> Player class constructor. </summary>
        /// <param name="a_Device">The active Irrlicht Device.</param>
        /// <param name="a_PhysicsWorld">The a_ physics world.</param>
        /// <param name="a_ID">The ID of the scenenode.</param>
        /// <param name="a_TeamIdentifier">The team's identifier the player should have.</param>
        /// <param name="a_MainPlayer">A boolean to identify the player the user controls.</param>
        /// <param name="a_AudioManager">The audio manager.</param>
        Player(irr::IrrlichtDevice* a_Device, ConfusShared::Physics::PhysicsWorld& a_PhysicsWorld, long long a_ID, 
			ConfusShared::ETeamIdentifier a_TeamIdentifier, bool a_MainPlayer, 
			Audio::AudioManager* a_AudioManager);
        /// <summary> Player class destructor. </summary>
		~Player();
        /// <summary> Update function, any tasks that need to be done every frame go here. </summary>
        void update();
        ///<summary> Respawns the player to their base, public so round resets etc. can call this. </summary>
        void respawn();
		/// <summary> Updates the color </summary>
		/// <param name="a_Device"> The active irrlicht device </param>
		void updateColor(irr::IrrlichtDevice* a_Device);
        /// <summary> Updates the rotation of this player </summary>
        /// <param name="a_NewRotation">The new rotation for this player.</param>
        void updateRotation(irr::core::vector3df a_NewRotation);
        /// <summary> Updates the position of this player. </summary>
        /// <param name="a_NewPosition">The new position for this player.</param>
        void updatePosition(irr::core::vector3df a_NewPosition);

        void fixedUpdate();
        /// <summary> Required render function for the ISceneNode, does nothing as we render in the Game.cpp.</summary>
        virtual void render();
        /// <summary> Returns the bounding box of the player's mesh. </summary>
        virtual const irr::core::aabbox3d<irr::f32> & getBoundingBox() const;
        /// <summary> Sets the connection to the server. </summary>
        void setConnection(Networking::ClientConnection* a_Connection);
		/// <summary> get/setter for health. Should really turn into const but gave some weird errors </summary>
		ConfusShared::Health* getHealthInstance();
        /// <summary> Sets the connection to the server. </summary>
        void updateServer() const;
		void onScore();
		/// <summary> Starts the light attack, dealing normal damage. </summary>
		void startLightAttack();

		/// <summary> Starts the heavy attack, which deals more damage. </summary>
		void startHeavyAttack();
    private:
		void dropFlag();
        /// <summary> Starts the walking animation, which is the default animation. </summary>
        void startWalking() const;
        /// <summary> Stops the walking animation. </summary>
        void stopWalking() const;
        /// <summary> Initializes the shared attack variables. </summary>
        void initializeAttack();
        
        /// <summary> Changes the state of the player. </summary>
        void changeState(EPlayerState a_NewState);

        /// <summary> Called when the animation finishes </summary>
        /// <remarks> Generally used for the attack animations only </remarks>
        /// <param name="node">The node whoms animation finished</param>
        virtual void OnAnimationEnd(irr::scene::IAnimatedMeshSceneNode* node) override;
    };
}
