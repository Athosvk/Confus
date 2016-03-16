#pragma once
#include <irrlicht/irrlicht.h>

#include "EventManager.h"
#include "Weapon.h"

namespace Confus
{
    class Player
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
        void fixedUpdate();
        void update();

    private:
        irr::SKeyMap m_KeyMap[5];
    };
}