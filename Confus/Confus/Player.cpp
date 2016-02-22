#include "Player.h"
#include <IrrAssimp/IrrAssimp.h>



Player::Player(irr::scene::ISceneManager * sceneManager)
{
    IrrAssimp irrAssimp(sceneManager);
    auto meshNode = irrAssimp.getMesh("Media/arm.fbx");
    auto sceneNode = sceneManager->addOctreeSceneNode(meshNode->getMesh(0), 0, -1, 1024);
    sceneNode->setParent(sceneManager->getActiveCamera());
    sceneNode->setPosition(irr::core::vector3d<float>(2.5f, 0.5f, 0.5f));
    sceneNode->setRotation(irr::core::vector3d<float>(45.0f, 0.0f, 0.0f));

    auto collisionBox = sceneManager->addCubeSceneNode(2.0f, sceneNode->getParent(), -1, irr::core::vector3d<float>(0.0f, -1.0f, 0.0f), irr::core::vector3d<float>(0.0f, 0.0f, 0.0f), irr::core::vector3d<float>(1.0f, 1.5f, 1.0f));
    collisionBox->setVisible(false);
}

void Player::fixedUpdate()
{
}

void Player::update()
{
}