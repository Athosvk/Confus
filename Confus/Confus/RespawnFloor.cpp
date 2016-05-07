#include "RespawnFloor.h"
#include "../ConfusShared/Physics/RigidBody.h"
#include "../ConfusShared/Physics/PhysicsWorld.h"
#include "../ConfusShared/Physics/BoxCollider.h"

namespace Confus
{
	RespawnFloor::RespawnFloor(irr::IrrlichtDevice* a_Device, Physics::PhysicsWorld& a_PhysicsWorld,
		irr::core::vector3df a_Position)
    {
        auto sceneManager = a_Device->getSceneManager();
        auto videoDriver = a_Device->getVideoDriver();

        m_RegularTexture = videoDriver->getTexture("Media/Textures/SquareWall.jpg");
        m_TransparentTexture = videoDriver->getTexture("Media/Textures/SquareWallTransparent.png");

        m_FloorNode = sceneManager->addAnimatedMeshSceneNode(sceneManager->getMesh("Media/BaseGlassFloor.irrmesh"));
        m_FloorNode->setMaterialType(irr::video::E_MATERIAL_TYPE::EMT_SOLID);
        m_FloorNode->setMaterialTexture(0, m_RegularTexture);
        m_FloorNode->setScale(irr::core::vector3df(5.5f, 0.4f, 10.f));
		m_FloorNode->setPosition(a_Position);
		m_RigidBody = a_PhysicsWorld.createBoxCollider(m_FloorNode, Physics::ECollisionFilter::Interactable,
			Physics::ECollisionFilter::All)->getRigidBody();
		m_RigidBody->makeStatic();
    }


    RespawnFloor::~RespawnFloor()
    {
    }

    void RespawnFloor::enableCollision()
    {
        m_FloorNode->setMaterialType(irr::video::E_MATERIAL_TYPE::EMT_SOLID);
        m_FloorNode->setMaterialTexture(0, m_RegularTexture);
		m_RigidBody->activate();
    }

    void RespawnFloor::disableCollision()
    {
        m_FloorNode->setMaterialType(irr::video::E_MATERIAL_TYPE::EMT_TRANSPARENT_ALPHA_CHANNEL);
        m_FloorNode->setMaterialTexture(0, m_TransparentTexture);
		m_RigidBody->deactivate();
    }
}

