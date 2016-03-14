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

        Player(irr::IrrlichtDevice* a_Device);
        void fixedUpdate();
        void update();

    private:
        irr::SKeyMap m_KeyMap[5];
    };
}