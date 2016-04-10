#include "RespawnFloor.h"

namespace Confus
{
    RespawnFloor::RespawnFloor(irr::IrrlichtDevice* a_Device)
    {
        auto sceneManager = a_Device->getSceneManager();
        auto videoDriver = a_Device->getVideoDriver();

        //TODO: Need floor model & textures (team colored?)
        m_Mesh = sceneManager->getMesh("Media/ninja.b3d");

        m_FloorNode = sceneManager->addAnimatedMeshSceneNode(m_Mesh, nullptr);
        m_FloorNode->setMaterialType(irr::video::E_MATERIAL_TYPE::EMT_SOLID);
        m_FloorNode->setMaterialTexture(0, videoDriver->getTexture("Media/nskinbl.jpg"));
        m_TriangleSelector = sceneManager->createTriangleSelector(m_FloorNode);
    }


    RespawnFloor::~RespawnFloor()
    {
    }

    void RespawnFloor::setPosition(irr::core::vector3df a_NewPosition)
    {
        m_FloorNode->setPosition(a_NewPosition);
    }

    void RespawnFloor::enableCollision()
    {
        m_FloorNode->setMaterialType(irr::video::E_MATERIAL_TYPE::EMT_SOLID);
        m_FloorNode->setTriangleSelector(m_TriangleSelector);
    }

    void RespawnFloor::disableCollision()
    {
        m_FloorNode->setMaterialType(irr::video::E_MATERIAL_TYPE::EMT_TRANSPARENT_ALPHA_CHANNEL);
        m_FloorNode->setTriangleSelector(nullptr);
    }
}

