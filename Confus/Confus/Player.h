#pragma once
#include <irrlicht/irrlicht.h>

namespace Confus
{
    class Player
    {
    public:
        irr::scene::IAnimatedMeshSceneNode* m_PlayerNode;
        Player(irr::IrrlichtDevice* device);
        void fixedUpdate();
        void update();
    };
}