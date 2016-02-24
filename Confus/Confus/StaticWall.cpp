#include "StaticWall.h"
#include <Irrlicht/irrlicht.h>
#include <IrrAssimp/IrrAssimp.h>

using namespace irr;

StaticWall::StaticWall(IrrlichtDevice* a_Device, core::vector3d<float> a_Position, core::vector3d<float> a_Rotation)
{
    scene::ISceneManager* smgr = a_Device->getSceneManager();
    IrrAssimp irrAssimp(smgr);
    
    auto meshNode = irrAssimp.getMesh("Media/Wall.fbx");
    auto sceneNode = smgr->addOctreeSceneNode(meshNode->getMesh(0), 0, -1, 1024);
    sceneNode->setPosition(a_Position);
    sceneNode->setRotation(a_Rotation);

    // Cannot test this yet.
    //auto collisionBox = smgr->addCubeSceneNode(10.0f, sceneNode->getParent(), -1, irr::core::vector3d<float>(0.0f, -1.0f, 0.0f), irr::core::vector3d<float>(0.0f, 0.0f, 0.0f), irr::core::vector3d<float>(1.0f, 1.5f, 1.0f));
    //collisionBox->setVisible(true);

    smgr->addCameraSceneNodeFPS();
    a_Device->getCursorControl()->setVisible(false);
}


StaticWall::~StaticWall()
{
}
