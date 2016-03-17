#pragma once
#include <irrlicht/irrlicht.h>

namespace Confus
{
    enum class ETeamIdentifier;
    enum class EFlagEnum;
    class Player
    {
    public:
        ETeamIdentifier TeamIdentifier;
        EFlagEnum CarryingFlag;
    public:
        irr::scene::IAnimatedMeshSceneNode* PlayerNode;
        Player(irr::IrrlichtDevice* a_Device);
        void fixedUpdate();
        void update();
    };
}