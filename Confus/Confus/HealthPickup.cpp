#include <Irrlicht/irrlicht.h>
#include <IrrAssimp/IrrAssimp.h>
#include <iostream>

#include "HealthPickup.h"
#include "Player.h"

#include "../ConfusShared/Physics/PhysicsWorld.h"
#include "../ConfusShared/Physics/BoxCollider.h"

namespace Confus
{
	HealthPickup::HealthPickup(irr::IrrlichtDevice* a_Device, Physics::PhysicsWorld& a_PhysicsWorld)
	{
		//Get drivers to load model
		auto sceneManager = a_Device->getSceneManager();
		auto videoDriver = a_Device->getVideoDriver();

		//Load model
		IrrAssimp irrAssimp(sceneManager);
		m_MeshNode = irrAssimp.getMesh("Media/Meshes/plus.blend");
		m_HealthPickupNode = sceneManager->addMeshSceneNode(m_MeshNode, 0, 2);
		m_HealthPickupNode->setMaterialFlag(irr::video::E_MATERIAL_FLAG::EMF_LIGHTING, false);
		m_HealthPickupNode->setScale({ 0.25f, 0.25f, 0.25f });
		m_Collider = a_PhysicsWorld.createBoxCollider(irr::core::vector3df(1.0f, 3.0f, 1.0f), m_HealthPickupNode, Physics::ECollisionFilter::Interactable,
			Physics::ECollisionFilter::All);
		m_Collider->getRigidBody()->makeKinematic();
		m_Collider->getRigidBody()->enableTriggerState();
		m_Collider->setTriggerEnterCallback([this](Physics::BoxCollider* a_Other)
		{
			auto collidedNode = a_Other->getRigidBody()->getAttachedNode();
			if (!collidedNode->getChildren().empty())
			{
				Player* player = dynamic_cast<Player*>(*collidedNode->getChildren().begin());
				if (player != nullptr && m_IsActive && player->getHealthInstance()->getHealth() != 100)
				{
					player->getHealthInstance()->heal(m_HealAmount);
					m_IsActive = false;
				}
			}
		});

		//Set Color
		setColor(videoDriver);

		//Add colission box and particle system
		initParticleSystem(sceneManager);
	}

	//Set color & position based on color of flag
	void HealthPickup::setColor(irr::video::IVideoDriver* a_VideoDriver)
	{
		m_HealthPickupNode->setMaterialTexture(0, a_VideoDriver->getTexture("Media/Textures/Pickup/HEALTH_PICKUP_GREEN.png"));
	}

	void HealthPickup::initParticleSystem(irr::scene::ISceneManager* a_SceneManager)
	{
		//Create Particle System
		irr::scene::IParticleSystemSceneNode* particleSystem = a_SceneManager->addParticleSystemSceneNode(false);
		auto colDark = getColor();
		auto colBright = colDark;
		colBright.setAlpha(0);

		//Emitter
		irr::scene::IParticleEmitter* particleEmitter = particleSystem->createBoxEmitter(
			irr::core::aabbox3d<irr::f32>(-7, 0, -7, 7, 1, 7),      // emitter size
			irr::core::vector3df(0.0f, 0.06f, 0.0f),                // initial direction
			40, 80,                                                 // emit rate
			colDark, colBright,                                     // dark en bright color
			40, 100, 0,                                             // min and max age, angle
			{ 0.25f, 0.25f }, { 0.5f, 0.5f });                      //min and max size

																	//Affector
		irr::scene::IParticleAffector* particleAffector = particleSystem->createFadeOutParticleAffector();
		particleSystem->setScale({ 0.05f, 0.05f, 0.05f });
		particleSystem->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		particleSystem->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
		particleSystem->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
		particleSystem->setParent(m_HealthPickupNode);
		particleSystem->setPosition({ 0.f, 0, 0 });

		//Drop systems after setting them
		particleSystem->setEmitter(particleEmitter);
		particleEmitter->drop();
		particleSystem->addAffector(particleAffector);
		particleAffector->drop();
	}

	const irr::video::SColor HealthPickup::getColor() const
	{
		return{ 170, 148, 255, 144 };
	}

	void HealthPickup::setPosition(irr::core::vector3df a_Position)
	{
		m_HealthPickupNode->setPosition(a_Position);
	}

	void HealthPickup::update(double a_DeltaTime)
	{
		irr::core::vector3df tempRotation;
		tempRotation = m_HealthPickupNode->getRotation() + irr::core::vector3df(0.f, 1.f, 0.f);
		m_HealthPickupNode->setRotation(tempRotation);

		if (!m_IsActive)
		{
			m_HealthPickupNode->setVisible(false);

			m_Timer += a_DeltaTime;
			if (m_Timer >= 10.0f)
			{
				m_HealthPickupNode->setVisible(true);
				m_IsActive = true;
				m_Timer = 0.0f;
			}
		}
	}

	HealthPickup::~HealthPickup()
	{

	}
}
