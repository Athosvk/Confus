#pragma once
#include <irrlicht/irrlicht.h>

#include "Weapon.h"

namespace Confus
{
    class EventManager;

    class Player : irr::scene::IAnimationEndCallBack
    {
    private:
        /// <summary> The weapon bone index of the animation for the weapon </summary>
        static const irr::u32 WeaponJointIndex;
        static const unsigned LightAttackDamage;
        static const unsigned HeavyAttackDamage;

    public:
        irr::scene::IAnimatedMeshSceneNode* PlayerNode;
        irr::scene::ICameraSceneNode* CameraNode;
    private:
        /// <summary> The player's weapon </summary>
        Weapon m_Weapon;
        /// <summary> Whether the player is currently attacking or not </summary>
        bool m_Attacking = false;

    public:
        Player(irr::IrrlichtDevice* a_Device);

        /// <summary> Handles the input based actions </summary>
        /// <param name="a_EventManager">The current event manager</param>
        void handleInput(EventManager& a_EventManager);
        void setLevelCollider(irr::scene::ISceneManager* a_SceneManager, irr::scene::ITriangleSelector* a_Level);
    private:
        /// <summary> Starts the walking animation, which is the default animation </summary>
        void startWalking() const;
        
        /// <summary> Initializes the shared attack variables </summary>
        void initializeAttack();

        /// <summary> Starts the light attack, dealing normal damage </summary>
        void startLightAttack();

        /// <summary> Starts the heavy attack, which deals more damage </summary>
        void startHeavyAttack();

        /// <summary> Called when the animation finishes </summary>
        /// <remarks> Generally used for the attack animations only </remarks>
        /// <param name="node">The node whoms animation finished</param>
        virtual void OnAnimationEnd(irr::scene::IAnimatedMeshSceneNode* node) override;
        irr::SKeyMap m_KeyMap[5];
    };
}