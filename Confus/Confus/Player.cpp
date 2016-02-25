#include <IrrAssimp/IrrAssimp.h>

#include "Player.h"

namespace Confus
{
    Player::Player(irr::scene::ISceneManager * sceneManager)
    {
    auto sceneManager = device->getSceneManager();
    auto videoDriver = device->getVideoDriver();

        IrrAssimp irrAssimp(sceneManager);
    irr::scene::IAnimatedMesh* mesh = sceneManager->getMesh("Media/ninja.b3d");

    m_PlayerNode = sceneManager->addAnimatedMeshSceneNode(mesh);
    m_PlayerNode->setMaterialFlag(irr::video::E_MATERIAL_FLAG::EMF_LIGHTING, false);
    m_PlayerNode->setMaterialTexture(0, videoDriver->getTexture("Media/nskinbl.jpg"));
    m_PlayerNode->setPosition(irr::core::vector3df(0, 0, 0));

    m_PlayerNode->setAnimationSpeed(24);
    m_PlayerNode->setFrameLoop(0, 13);
    m_PlayerNode->setCurrentFrame(7);

    auto camera = sceneManager->addCameraSceneNode(m_PlayerNode, irr::core::vector3df(0, 10, -15));

    auto collisionBox = sceneManager->addCubeSceneNode(1.0f, m_PlayerNode, -1, irr::core::vector3d<float>(0.0f, 5.0f, 0.0f), irr::core::vector3d<float>(0.0f, 0.0f, 0.0f), irr::core::vector3d<float>(2.5f, 10.0f, 2.0f));
    collisionBox->setVisible(false);
}

    void Player::fixedUpdate()
    {
    }

    void Player::update()
    {
    //m_PlayerNode->setPosition(m_PlayerNode->getPosition() + irr::core::vector3df(0, 0, 0.1f));
}