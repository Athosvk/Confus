#include <Irrlicht/irrlicht.h>
#include <IrrAssimp/IrrAssimp.h>

#include "StaticWall.h"

StaticWall::StaticWall(irr::IrrlichtDevice* a_Device, irr::core::vector3d<float> a_Position, irr::core::vector3d<float> a_Rotation)
{
    irr::scene::ISceneManager* sceneManager = a_Device->getSceneManager();
    IrrAssimp irrAssimp(sceneManager);
    
    auto meshNode = irrAssimp.getMesh("Media/Wall.fbx");
   
    auto sceneNode = sceneManager->addOctreeSceneNode(meshNode->getMesh(0), 0, -1, 1024);
    sceneNode->setPosition(a_Position);
    sceneNode->setRotation(a_Rotation);
}


StaticWall::~StaticWall()
{
}
