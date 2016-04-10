#pragma once

#include <irrlicht/irrlicht.h>

namespace Confus 
{
    class RespawnFloor
    {
    private:
        irr::scene::IAnimatedMeshSceneNode* m_FloorNode;
        irr::scene::IAnimatedMesh* m_Mesh;
        irr::scene::ITriangleSelector* m_TriangleSelector;
    public:
        RespawnFloor(irr::IrrlichtDevice* a_Device);
        ~RespawnFloor();
        void setPosition(irr::core::vector3df a_NewPosition);
        void enableCollision();
        void disableCollision();
    };
}


