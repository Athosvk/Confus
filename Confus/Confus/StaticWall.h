#pragma once
#include <Irrlicht/irrlicht.h>

class StaticWall
{
    irr::scene::ISceneManager* m_SceneManager;
    irr::scene::IAnimatedMesh* m_MeshNode;
    irr::scene::ISceneNode* m_SceneNode;
    irr::scene::ICameraSceneNode* m_Camera;
    irr::video::IVideoDriver* m_VideoDriver;

public:
    StaticWall(irr::IrrlichtDevice* a_Device, irr::core::vector3d<float> a_Position, irr::core::vector3d<float> a_Rotation, irr::scene::ICameraSceneNode* a_Camera);

    void loadMesh();

    void setTexture();

    void setSceneNode(irr::core::vector3d<float> a_Position, irr::core::vector3d<float> a_Rotation);

    void addMeshCollider();
    void addLight(irr::core::vector3df a_Position);
};

