#include "RespawnFloor.h"

namespace Confus
{
    RespawnFloor::RespawnFloor(irr::IrrlichtDevice* a_Device)
    {
        auto sceneManager = a_Device->getSceneManager();
        auto videoDriver = a_Device->getVideoDriver();

        m_RegularTexture = videoDriver->getTexture("Media/Textures/SquareWall.jpg");
        m_TransparentTexture = videoDriver->getTexture("Media/Textures/SquareWallTransparent.png");

        m_FloorNode = sceneManager->addAnimatedMeshSceneNode(sceneManager->getMesh("Media/BaseGlassFloor.irrmesh"), nullptr);
        m_FloorNode->setMaterialType(irr::video::E_MATERIAL_TYPE::EMT_SOLID);
        m_FloorNode->setMaterialTexture(0, m_RegularTexture);
        m_FloorNode->setScale(irr::core::vector3df(5.5f, 0.1f, 10.f));
        m_TriangleSelector = sceneManager->createTriangleSelector(m_FloorNode);
        m_FloorNode->setTriangleSelector(m_TriangleSelector);
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
        m_FloorNode->setMaterialTexture(0, m_RegularTexture);
    }

    void RespawnFloor::disableCollision()
    {
        m_FloorNode->setMaterialType(irr::video::E_MATERIAL_TYPE::EMT_TRANSPARENT_ALPHA_CHANNEL);
        m_FloorNode->setTriangleSelector(nullptr);
        m_FloorNode->setMaterialTexture(0, m_TransparentTexture);
    }
}

