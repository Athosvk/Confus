#pragma once
#include <irrlicht/irrlicht.h>

#include "EventManager.h"

namespace Confus
{
    class Player
    {
    public:
        irr::scene::IAnimatedMeshSceneNode* PlayerNode;
        irr::scene::ICameraSceneNode* CameraNode;
        irr::SKeyMap m_KeyMap[5];

        Player(irr::IrrlichtDevice* a_Device);
        void fixedUpdate();
        void update();
    };
}