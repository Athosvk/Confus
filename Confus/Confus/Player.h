#pragma once
#include <irrlicht/irrlicht.h>

#include "Weapon.h"

namespace Confus
{
    class EventManager;

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

        void handleInput(EventManager& a_EventManager);
    private:
        irr::SKeyMap m_KeyMap[5];
    };
}