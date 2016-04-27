#include <Irrlicht/irrlicht.h>
#include <IrrAssimp/IrrAssimp.h>
#include <iostream>

#include "Flag.h"
#include "Player.h"
#include "Collider.h"
#include "Debug.h"
#include "../ConfusShared/Physics/PhysicsWorld.h"
#include "../ConfusShared/Physics/BoxCollider.h"

namespace Confus {

	Flag::Flag(irr::IrrlichtDevice* a_Device, ETeamIdentifier a_TeamIdentifier, Physics::PhysicsWorld& a_PhysicsWorld) : 
		m_TeamIdentifier(new ETeamIdentifier(a_TeamIdentifier)),
		m_FlagStatus(new EFlagEnum(EFlagEnum::FlagBase)) 
	{
        //Get drivers to load model
        auto sceneManager = a_Device->getSceneManager();
        auto videoDriver = a_Device->getVideoDriver();

		//Flags
		m_StartPosition = new irr::core::vector3df();
		m_StartRotation = new irr::core::vector3df();

        //Load model
        IrrAssimp irrAssimp(sceneManager);
        irr::scene::IAnimatedMesh* mesh = sceneManager->getMesh("Media/Meshes/Flag.3ds");
        m_FlagNode = sceneManager->addMeshSceneNode(mesh, 0, 2);
        m_FlagNode->setMaterialFlag(irr::video::E_MATERIAL_FLAG::EMF_LIGHTING, false);
        m_FlagNode->setScale({ 1.5f, 1.5f, 1.5f });
		m_Collider = a_PhysicsWorld.createBoxCollider(irr::core::vector3df(1.0f, 3.0f, 1.0f), m_FlagNode, Physics::ECollisionFilter::Interactable,
			Physics::ECollisionFilter::All);
		m_Collider->getRigidBody()->makeKinematic();
		m_Collider->getRigidBody()->enableTriggerState();
		m_Collider->setTriggerEnterCallback([this](Physics::BoxCollider* a_Other)
		{
			auto collidedNode = a_Other->getRigidBody()->getAttachedNode();
			Player* player = dynamic_cast<Player*>(*collidedNode->getChildren().begin());
			if(player != nullptr)
			{
				captureFlag(player);
			}
		});

        m_FlagOldParent = m_FlagNode->getParent();

        //Set Color
		setColor(videoDriver);

        //Add colission box and particle system
        initParticleSystem(sceneManager);
	}

    void Flag::setCollisionTriangleSelector(irr::scene::ISceneManager* a_SceneManager, irr::scene::ITriangleSelector* a_TriangleSelector) 
    {
        auto animator = a_SceneManager->createCollisionResponseAnimator(a_TriangleSelector, m_FlagNode, { 1.25f, 1.f, 1.25f });
        m_FlagNode->addAnimator(animator);
    }

	//Set color & position based on color of flag
	void Flag::setColor(irr::video::IVideoDriver* a_VideoDriver) 
	{
		switch (*m_TeamIdentifier)
		{
		case ETeamIdentifier::TeamBlue:
            m_FlagNode->setMaterialTexture(0, a_VideoDriver->getTexture("Media/Textures/Flag/FLAG_BLUE.png"));
			m_StartPosition->set({ -2.0f, 15.f, -2.f });
			m_StartRotation->set({ 0.f, 0.f, 0.f });
			returnToStartPosition();
			break;
		case ETeamIdentifier::TeamRed:
            m_FlagNode->setMaterialTexture(0, a_VideoDriver->getTexture("Media/Textures/Flag/FLAG_RED.png"));
			m_StartPosition->set({ 1.5f, 15.f, -72.f });
			m_StartRotation->set({ 0.f, 180.f, 0.f });
            returnToStartPosition();
			break;
		default:
			m_StartPosition->set({ 0, 0, 0 });
			m_StartRotation->set({ 0, 0, 0 });
			break;
		}
	}

    void Flag::initParticleSystem(irr::scene::ISceneManager* a_SceneManager) 
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
            400, 1000, 0,                                           // min and max age, angle
            { 0.5f, 0.5f }, { 1.f, 1.f });                          //min and max size

        //Affector
        irr::scene::IParticleAffector* particleAffector = particleSystem->createFadeOutParticleAffector();
        particleSystem->setScale({ 0.1f, 0.1f, 0.1f });
        particleSystem->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        particleSystem->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
        particleSystem->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
        particleSystem->setParent(m_FlagNode);
        particleSystem->setPosition({ 1.0f, 0, 0 });

        //Drop systems after setting them
        particleSystem->setEmitter(particleEmitter);
        particleEmitter->drop();
        particleSystem->addAffector(particleAffector);
        particleAffector->drop();
    }

     const irr::video::SColor Flag::getColor() const
    {
        switch(*m_TeamIdentifier)
        {
        case ETeamIdentifier::TeamBlue:
            return { 255, 0, 0, 255 };
        case ETeamIdentifier::TeamRed:
            return { 255, 255, 0, 0 };
        default:
            return { 255, 255, 255, 255 };
        }
    }

	 const EFlagEnum * Flag::getFlagStatus() const
	 {
		 return m_FlagStatus;
	 }

	//This class handles what to do on collision
	void Flag::captureFlag(Player* a_PlayerObject) 
    {
		//Somebody is already carrying the flag
		if (*m_FlagStatus == EFlagEnum::FlagTaken) 
		{
			return;
		}


		if (*a_PlayerObject->TeamIdentifier != *m_TeamIdentifier && *a_PlayerObject->CarryingFlag == EFlagEnum::None) 
        {
            // Capturing flag if player has no flag
            m_FlagNode->setParent(a_PlayerObject->PlayerNode);            
            *m_FlagStatus = EFlagEnum::FlagTaken;
            a_PlayerObject->FlagPointer = this;
            *a_PlayerObject->CarryingFlag = EFlagEnum::FlagTaken;
		}
		else if (*a_PlayerObject->TeamIdentifier == *m_TeamIdentifier) 
        {
			//If flag has been dropped return flag to base
 			if (*m_FlagStatus == EFlagEnum::FlagDropped) 
            {
                returnToStartPosition();
			}
			//If flag is at base and player is carrying a flag
			else if (*m_FlagStatus == EFlagEnum::FlagBase) 
            {
				if (*a_PlayerObject->CarryingFlag == EFlagEnum::FlagTaken) 
                {					
                    if(a_PlayerObject->FlagPointer != nullptr) 
					{
                        // Player scored a point!
                        a_PlayerObject->FlagPointer->returnToStartPosition();
                        a_PlayerObject->FlagPointer = nullptr;
                        score(a_PlayerObject);
                    }
					else
					{
						throw std::exception("Failed to get the flag from the player object");
					}
				}
			}
		}
	}

	//TODO Score points to team of a_PlayerObject
	void Flag::score(Player* a_PlayerObject) 
    {
        *a_PlayerObject->CarryingFlag = EFlagEnum::None;
	}

	void Flag::drop(Player* a_PlayerObject) 
    {
        m_FlagNode->setParent(m_FlagOldParent);
        m_FlagNode->setPosition(a_PlayerObject->PlayerNode->getAbsolutePosition());
        a_PlayerObject->FlagPointer = nullptr;
        *m_FlagStatus = EFlagEnum::FlagDropped;
        *a_PlayerObject->CarryingFlag = EFlagEnum::None;
	}

    void Flag::setStartPosition(irr::core::vector3df a_Position) 
    {
        m_StartPosition->set(a_Position);
    }

    void Flag::setStartRotation(irr::core::vector3df a_Rotation) 
    {
        m_StartRotation->set(a_Rotation);
    }

    void Flag::returnToStartPosition() {
        m_FlagNode->setParent(m_FlagOldParent);
        m_FlagNode->setPosition(*m_StartPosition);
        m_FlagNode->setRotation(*m_StartRotation);
		*m_FlagStatus = EFlagEnum::FlagBase;
    }

	irr::scene::ITriangleSelector* Flag::GetTriangleSelector(irr::scene::ISceneManager* a_SceneManager) {
		return a_SceneManager->createTriangleSelectorFromBoundingBox(m_FlagNode);
	}

	Flag::~Flag() {
        m_FlagNode->setParent(m_FlagOldParent);
		delete(m_TeamIdentifier);
		delete(m_FlagStatus);
		delete(m_StartPosition);
        delete(m_StartRotation);
	}
}