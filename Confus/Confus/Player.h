#pragma once
#include <irrlicht/irrlicht.h>

#include "Weapon.h"

namespace Confus
{
    class EventManager;

    class Player : irr::scene::IAnimationEndCallBack
    {
    private:
        /// <summary>
        /// The weapon joint index
        /// </summary>
        static const irr::u32 WeaponJointIndex;
    public:
        irr::scene::IAnimatedMeshSceneNode* PlayerNode;
        irr::scene::ICameraSceneNode* CameraNode;
    private:
        /// <summary>
        /// The player's weapon
        /// </summary>
        Weapon m_Weapon;
        /// <summary>
        /// Whether the player is currently attacking or not
        /// </summary>
        bool m_Attacking = false;

    public:
        Player(irr::IrrlichtDevice* a_Device);

        /// <summary> Handles the input based actions </summary>
        /// <param name="a_EventManager">The current event manager</param>
        void handleInput(EventManager& a_EventManager);
    private:
        /// <summary> Starts the walking animation, which is the default animation </summary>
        void startWalking();

        /// <summary> Starts the regular attack </summary>
        void startAttack();

        /// <summary> Starts the heavy attack, which deals more damage </summary>
        void startHeavyAttack();

        /// <summary>
        /// Called when the animation finishes
        /// <remarks> Generally used for the attack animations only </remarks>
        /// </summary>
        /// <param name="node">The node whoms animation finished</param>
        virtual void OnAnimationEnd(irr::scene::IAnimatedMeshSceneNode* node) override;
        irr::SKeyMap m_KeyMap[5];
    };
}