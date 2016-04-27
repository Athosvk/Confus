#include <IrrAssimp/IrrAssimp.h>

#include "StaticWall.h"

namespace Confus
{
    StaticWall::StaticWall(irr::IrrlichtDevice* a_Device, irr::core::vector3d<float> a_Position, irr::core::vector3d<float> a_Rotation, irr::scene::ICameraSceneNode* a_Camera)
    {
        m_SceneManager = a_Device->getSceneManager();
        m_VideoDriver = a_Device->getVideoDriver();
        m_Camera = a_Camera;

        loadMesh();
        setSceneNode(a_Position, a_Rotation);
        setTexture();
        addMeshCollider();
        addLight(a_Position);
    }

    StaticWall::~StaticWall()
    {
        m_MeshNode->drop();
        m_SceneNode->drop();
    }

    void StaticWall::loadMesh()
    {
        IrrAssimp irrAssimp(m_SceneManager);
        m_MeshNode = irrAssimp.getMesh("Media/Models/Wall.3DS");
    }

    void StaticWall::setSceneNode(irr::core::vector3d<float> a_Position, irr::core::vector3d<float> a_Rotation)
    {
        m_SceneNode = m_SceneManager->addOctreeSceneNode(m_MeshNode->getMesh(0), 0, -1, 1024);
        m_SceneNode->setPosition(a_Position);
        m_SceneNode->setRotation(a_Rotation);
    }

    void StaticWall::setTexture()
    {
        m_SceneNode->setMaterialTexture(0, m_VideoDriver->getTexture("Media/Textures/Wall_texture.png"));
        // Enable dynamic lighting.
        m_SceneNode->setMaterialFlag(irr::video::EMF_LIGHTING, true);
    }

    void StaticWall::addLight(irr::core::vector3df a_Position)
    {
        irr::core::vector3df lightOffset = irr::core::vector3df(0, 10, 50);
        m_SceneManager->addLightSceneNode(m_SceneNode, a_Position + lightOffset,
            irr::video::SColorf(1.0f, 1.0f, 1.0f), 50.0f);
    }
}




