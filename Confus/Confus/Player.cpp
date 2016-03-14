#include <IrrAssimp/IrrAssimp.h>

#include "Player.h"
namespace Confus
{
    Player::Player(irr::IrrlichtDevice* a_Device)
    {
        auto sceneManager = a_Device->getSceneManager();
        auto videoDriver = a_Device->getVideoDriver();

        IrrAssimp irrAssimp(sceneManager);
        irr::scene::IAnimatedMesh* mesh = sceneManager->getMesh("Media/ninja.b3d");

        PlayerNode = sceneManager->addAnimatedMeshSceneNode(mesh);
        PlayerNode->setMaterialFlag(irr::video::E_MATERIAL_FLAG::EMF_LIGHTING, false);
        PlayerNode->setMaterialTexture(0, videoDriver->getTexture("Media/nskinbl.jpg"));
        PlayerNode->setPosition(irr::core::vector3df(0, 0, 0));

        PlayerNode->setAnimationSpeed(24);
        PlayerNode->setFrameLoop(0, 13);
        PlayerNode->setCurrentFrame(7);

        auto camera = sceneManager->addCameraSceneNode(NULL, irr::core::vector3df(0, 10, -15));


        auto collisionBox = sceneManager->addCubeSceneNode(1.0f, PlayerNode, -1, irr::core::vector3d<float>(0.0f, 5.0f, 0.0f), irr::core::vector3d<float>(0.0f, 0.0f, 0.0f), irr::core::vector3d<float>(2.5f, 10.0f, 2.0f));
        collisionBox->setVisible(false);

        createAudioEmitter();
    }

    void Player::fixedUpdate()
    {
    }

    void Player::update()
    {
        m_FootstepSoundEmitter->updatePosition();

        if(static_cast<int>(PlayerNode->getFrameNr()) == 3)
        {
            m_FootstepSoundEmitter->playFootStepSound();
        }
    }

    void Player::createAudioEmitter()
    {
        m_FootstepSoundEmitter = new Audio::PlayerAudioEmitter(PlayerNode);
    }
}