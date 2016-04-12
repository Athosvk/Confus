#pragma once

#include <irrlicht/irrlicht.h>

namespace Confus 
{
    class RespawnFloor
    {
    private:
        irr::scene::IAnimatedMeshSceneNode* m_FloorNode;
        irr::scene::ITriangleSelector* m_TriangleSelector;
        irr::video::ITexture* m_RegularTexture;
        irr::video::ITexture* m_TransparentTexture;
    public:
        RespawnFloor(irr::IrrlichtDevice* a_Device);
        ~RespawnFloor();
        void setPosition(irr::core::vector3df a_NewPosition);
        void enableCollision();
        void disableCollision();
    };
}


