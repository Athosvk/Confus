#include <IrrAssimp/IrrAssimp.h>

#include "Player.h"
#include "Collider.h"

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
		PlayerNode->setPosition(irr::core::vector3df(0, 0, -35));
		PlayerNode->setAnimationSpeed(24);
		PlayerNode->setFrameLoop(0, 13);
		PlayerNode->setCurrentFrame(7);

		auto camera = sceneManager->addCameraSceneNodeFPS(NULL, 100.f, 0.01f, 1, NULL, NULL, true, 0.03f);
		camera->setPosition({0, 1.5f, 0});
		PlayerNode->setParent(camera);

		auto collisionBox = sceneManager->addCubeSceneNode(1.0f, camera, static_cast<irr::s32>(1), irr::core::vector3d<float>(0.0f, 5.0f, 0.0f), irr::core::vector3d<float>(0.0f, 0.0f, 0.0f), irr::core::vector3d<float>(2.5f, 10.0f, 2.0f));
		collisionBox->setVisible(false);	
	}


    void Player::fixedUpdate()
    {
    }

    void Player::update()
    {

    }
}