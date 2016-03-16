#pragma once
#include <irrlicht/irrlicht.h>

#include "Weapon.h"

namespace Confus
{
    class EventManager;

    class Player : irr::scene::IAnimationEndCallBack
    {
    private:
        static const irr::u32 WeaponJointIndex;
    public:
        irr::scene::IAnimatedMeshSceneNode* PlayerNode;
        irr::scene::ICameraSceneNode* CameraNode;
    private:
        Weapon m_Weapon;

    public:
        Player(irr::IrrlichtDevice* a_Device);

        void handleInput(EventManager& a_EventManager);
    private:
        void startWalking();
        void startAttack();
        void startHeavyAttack();
        virtual void OnAnimationEnd(irr::scene::IAnimatedMeshSceneNode* node) override;
        irr::SKeyMap m_KeyMap[5];
    };
}