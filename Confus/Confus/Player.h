#pragma once
#include <irrlicht/irrlicht.h>

namespace Confus
{
    enum class TeamIdentifier;
    enum class FlagEnum;
    class Player
    {
    public:
        TeamIdentifier m_TeamIdentifier;
        FlagEnum m_CarryingFlag;
    public:
        irr::scene::IAnimatedMeshSceneNode* PlayerNode;
        Player(irr::IrrlichtDevice* a_Device);
        void fixedUpdate();
        void update();
    };
}