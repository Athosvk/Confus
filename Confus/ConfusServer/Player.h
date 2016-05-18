#pragma once
#include <irrlicht/irrlicht.h>
#include <RakNet/RakNetSocket2.h>

#include "Networking\Connection.h"
#include "../ConfusShared/Health.h"
#include "Weapon.h"
#include "../ConfusShared/TeamIdentifier.h"

namespace ConfusServer {

    namespace Physics
    {
        class BoxCollider;
        class PhysicsWorld;
    }

	namespace Audio 
    {
		class PlayerAudioEmitter;
	}

    namespace Networking
    {
        class Connection;
    }

    enum class EFlagEnum;
    class EventManager;
    class Flag;
    
    enum class EPlayerState : unsigned char
    {
        Alive, CarryingFlag, HeavyAttacking, LightAttacking, Dead
    };
    class Player : irr::scene::IAnimationEndCallBack, public irr::scene::ISceneNode
    {   
    public:
		/// <summary> The IAnimatedMeshSceneNode for the player </summary>
        irr::scene::IAnimatedMeshSceneNode* PlayerNode;
        irr::scene::ICameraSceneNode* CameraNode = nullptr;
		EFlagEnum CarryingFlag;
		ConfusShared::ETeamIdentifier TeamIdentifier;
        Flag* FlagPointer;
        Health PlayerHealth;
        long long ID;
        /// <summary> The systemaddress of the client player that matches this player. </summary>
        RakNet::SystemAddress SystemAddress;
	private:
        Audio::PlayerAudioEmitter* m_FootstepSoundEmitter;
        ConfusShared::Physics::BoxCollider* m_Collider;
        void createAudioEmitter();
        void updateClient();
        void moveOnButtonPress(irr::EKEY_CODE a_Key);
        /// <summary> The weapon bone index of the animation for the weapon </summary>
        static const irr::u32 WeaponJointIndex;
        static const unsigned LightAttackDamage;
        static const unsigned HeavyAttackDamage;
	    
	    ConfusShared::Health PlayerHealth;
        /// <summary> The player's weapon </summary>
        Weapon m_Weapon;
        /// <summary> Whether the player is currently attacking or not </summary>
        bool m_Attacking = false;
        /// <summary> The player's mesh </summary>
        irr::scene::IAnimatedMesh* m_Mesh;
        /// <summary> A pointer to the connection to the client. </summary>
        Networking::Connection* m_Connection;
    public:
        Player(irr::IrrlichtDevice* a_Device, long long a_id, ConfusShared::ETeamIdentifier a_TeamIdentifier, bool a_MainPlayer, RakNet::SystemAddress a_Address);
		~Player();
        void fixedUpdate();
        void update();
        virtual void render();
        /// <summary> Returns the bounding box of the player's mesh </summary>
        virtual const irr::core::aabbox3d<irr::f32> & getBoundingBox() const;
        /// <summary> Handles the input based actions </summary>
        /// <param name="a_EventManager">The current event manager</param>
        void handleInput(EventManager& a_EventManager);
        void setLevelCollider(irr::scene::ISceneManager* a_SceneManager, irr::scene::ITriangleSelector* a_Level);
        /// <summary> Starts the light attack, dealing normal damage </summary>
        void startLightAttack();
        /// Reset the player
        void resetPlayer();

        /// <summary> Starts the heavy attack, which deals more damage </summary>
        void startHeavyAttack();

        void setConnection(Networking::Connection* a_Connection);
    private:
        /// <summary> Starts the walking animation, which is the default animation </summary>
        void startWalking() const;
        
        /// <summary> Initializes the shared attack variables </summary>
        void initializeAttack();

        /// <summary> Called when the animation finishes </summary>
        /// <remarks> Generally used for the attack animations only </remarks>
        /// <param name="node">The node whoms animation finished</param>
        virtual void OnAnimationEnd(irr::scene::IAnimatedMeshSceneNode* node) override;
        irr::SKeyMap m_KeyMap[6];
    };
}
