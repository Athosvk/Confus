#pragma once
#include <irrlicht/irrlicht.h>

#include "Health.h"
#include "Weapon.h"
#include "../ConfusShared/TeamIdentifier.h"
#include "../ConfusShared/Networking/BitStreamStruct.h"

#include "RakNet/RakNetSocket2.h"
#include "Networking/Connection.h"

namespace ConfusServer
{
	namespace Audio 
    {
		class PlayerAudioEmitter;
	}
    enum class EFlagEnum;
    class EventManager;
    class Flag;
   
    class Player : irr::scene::IAnimationEndCallBack, public irr::scene::ISceneNode
    {   
    public:
		/// <summary> The IAnimatedMeshSceneNode for the player </summary>
        irr::scene::IAnimatedMeshSceneNode* PlayerNode;
        irr::scene::ICameraSceneNode* CameraNode = nullptr;
		EFlagEnum CarryingFlag;
		ETeamIdentifier TeamIdentifier;    
        Flag* FlagPointer;
        long long ID;
        ConfusShared::Networking::EPlayerState PlayerState = ConfusShared::Networking::EPlayerState::Alive;
        Health PlayerHealth;
   
	private:
		Audio::PlayerAudioEmitter* m_FootstepSoundEmitter;
        /// <summary> The weapon bone index of the animation for the weapom_PlayerStaten </summary>
        static const irr::u32 WeaponJointIndex;
        static const unsigned LightAttackDamage;
        static const unsigned HeavyAttackDamage;
        /// <summary> The player's weapon </summary>
        Weapon m_Weapon;
        /// <summary> Whether the player is currently attacking or not </summary>
        bool m_Attacking = false;
        /// <summary> The player's mesh </summary>
        irr::scene::IAnimatedMesh* m_Mesh;
        /// <summary> A pointer to the connection to the client. </summary>
        Networking::Connection* m_Connection;
		irr::SKeyMap m_KeyMap[6];
    public:
        Player(irr::IrrlichtDevice* a_Device, long long a_id, ETeamIdentifier a_TeamIdentifier, bool a_MainPlayer);
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

        /// <summary> Starts the heavy attack, which deals more damage </summary>
        void startHeavyAttack();
        
        /// <summary>
        /// Sets the connection to submit the update bitstream to.
        /// </summary>
        /// <param name="a_Connection">The connection reference.</param>
        void setConnection(Networking::Connection* a_Connection);
    private:
        /// <summary> Starts the walking animation, which is the default animation </summary>
        void startWalking() const;
        
        /// <summary> Initializes the shared attack variables </summary>
        void initializeAttack();

		void createAudioEmitter();
		void updateClient();
		void moveOnButtonPress(irr::EKEY_CODE a_Key);

        /// <summary> Called when the animation finishes </summary>
        /// <remarks> Generally used for the attack animations only </remarks>
        /// <param name="node">The node whoms animation finished</param>
        virtual void OnAnimationEnd(irr::scene::IAnimatedMeshSceneNode* node) override;
    };
}
