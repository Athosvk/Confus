#pragma once
#include <irrlicht/irrlicht.h>

#include "EventManager.h"

namespace Confus
{
    class Player
    {
    public:
        irr::scene::IAnimatedMeshSceneNode* PlayerNode;

        Player(irr::IrrlichtDevice* a_Device);
        void fixedUpdate();
        void update();
    };
}