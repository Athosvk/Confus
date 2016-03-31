#include <irrlicht/irrlicht.h>
#include <IrrAssimp/IrrAssimp.h>

#include "Flag.h"
#include "Player.h"
#include "Collider.h"


namespace Confus {

	Flag::Flag(irr::IrrlichtDevice* a_Device, ETeamIdentifier a_TeamIdentifier) : m_TeamIdentifier(&a_TeamIdentifier) {
        //Get drivers to load model
        auto sceneManager = a_Device->getSceneManager();
        auto videoDriver = a_Device->getVideoDriver();

		//Flags
		m_FlagStatus = new EFlagEnum(EFlagEnum::FLAG_BASE);
		m_StartPosition = new irr::core::vector3df();
		m_StartRotation = new irr::core::vector3df();

        //Load model
        IrrAssimp irrAssimp(sceneManager);
        irr::scene::IAnimatedMesh* mesh = sceneManager->getMesh("Media/Meshes/Flag.3ds");
        m_FlagNode = sceneManager->addAnimatedMeshSceneNode(mesh);
        m_FlagNode->setMaterialFlag(irr::video::E_MATERIAL_FLAG::EMF_LIGHTING, false);
        m_FlagNode->setScale({ 2, 2, 2 });

        //Set Color
		setColor(videoDriver);

        //Add colission box and particle system
        auto collisionBox = sceneManager->addCubeSceneNode(1.0f, m_FlagNode, 2, irr::core::vector3d<float>(1.f, 2.0f, 0.0f), irr::core::vector3d<float>(0.0f, 0.0f, 0.0f), irr::core::vector3d<float>(2.0f, 4.0f, 1.0f));
        collisionBox->setVisible(false);
        initParticleSystem(sceneManager);

		//Add collision callback
		auto anim = sceneManager->createCollisionResponseAnimator(sceneManager->getActiveCamera()->getTriangleSelector(), collisionBox);
		Collider col(anim);
		col.setCallback([this](irr::scene::ISceneNode* a_CollidedNode) 
		{
			if ((a_CollidedNode->getID() & 1) == 1) 
			{
				captureFlag(NULL);
				return true;
			}
			else if ((a_CollidedNode->getID() & 2) == 2) 
			{
				captureFlag(NULL);
				return true;
			}
			return false;
		});
	}


	//Set color & position based on color of flag
	void Flag::setColor(irr::video::IVideoDriver* a_VideoDriver) 
	{
		switch (*m_TeamIdentifier)
		{
		case ETeamIdentifier::TEAM_BLUE:
			m_FlagNode->setMaterialTexture(0, a_VideoDriver->getTexture("Media/Textures/Flag/FLAG_BLUE.png"));
			m_StartPosition->set({ -2.5f, 3.5f, -2.f });
			m_StartRotation->set({ 0.f, 0.f, 0.f });
			returnToStartPos();
			break;
		case ETeamIdentifier::TEAM_RED:
			m_FlagNode->setMaterialTexture(0, a_VideoDriver->getTexture("Media/Textures/Flag/FLAG_RED.png"));
			m_StartPosition->set({ 3.5f, 3.5f, -72.f });
			m_StartRotation->set({ 0.f, 180.f, 0.f });
			returnToStartPos();
			break;
		default:
			m_StartPosition->set({ 0, 0, 0 });
			m_StartRotation->set({ 0, 0, 0 });
			break;
		}
	}

    void Flag::initParticleSystem(irr::scene::ISceneManager* a_SceneManager) {
        //Create Particle System
        irr::scene::IParticleSystemSceneNode* particleSystem = a_SceneManager->addParticleSystemSceneNode(false);
        auto colDark = getColor();
        auto colBright = colDark;
        colBright.setAlpha(0);

        //Emitter
        irr::scene::IParticleEmitter* particleEmitter = particleSystem->createBoxEmitter(
            irr::core::aabbox3d<irr::f32>(-7, 0, -7, 7, 1, 7),      // emitter size
            irr::core::vector3df(0.0f, 0.06f, 0.0f),                // initial direction
            40, 50,                                                 // emit rate
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

    irr::video::SColor Flag::getColor() {
        switch(*m_TeamIdentifier)
        {
        case ETeamIdentifier::TEAM_BLUE:
            return { 255, 0, 0, 255 };
        case ETeamIdentifier::TEAM_RED:
            return { 255, 255, 0, 0 };
        default:
            return { 255, 255, 255, 255 };
        }
    }

	//This class handles what to do on collision
	void Flag::captureFlag(Player* a_PlayerObject) {
		//Somebody is already carrying the flag
		if (*m_FlagStatus == EFlagEnum::FLAG_TAKEN)
			return;

		if (a_PlayerObject->TeamIdentifier != *m_TeamIdentifier) {
			//Capture Flag
			m_FlagNode->setParent(a_PlayerObject->PlayerNode);
		}
		else if (a_PlayerObject->TeamIdentifier == *m_TeamIdentifier) {
			//If flag has been dropped return flag to base
			if (*m_FlagStatus == EFlagEnum::FLAG_DROPPED) {
				m_FlagNode->setParent(NULL);
				returnToStartPos();
			}
			//If flag is at base and player is carrying a flag
			else if (*m_FlagStatus == EFlagEnum::FLAG_BASE) {
				if (a_PlayerObject->CarryingFlag == EFlagEnum::FLAG_TAKEN) {
					score(a_PlayerObject);
				}
			}
		}
	}

	///Score points to team of a_PlayerObject
	void Flag::score(Player* a_PlayerObject) {
		
	}

	///Drop Flag at position of player
	void Flag::drop(Player* a_PlayerObject) {
		
	}

    void Flag::setStartPosition(irr::core::vector3df a_Position) {
        m_StartPosition->set(a_Position);
    }

    void Flag::setStartRotation(irr::core::vector3df a_Rotation) {
        m_StartRotation->set(a_Rotation);
    }

    void Flag::returnToStartPos() {
        m_FlagNode->setPosition(*m_StartPosition);
        m_FlagNode->setRotation(*m_StartRotation);
		*m_FlagStatus = EFlagEnum::FLAG_BASE;
    }

	Flag::~Flag() {
		delete(m_FlagStatus);
		delete(m_StartPosition);
		delete(m_StartRotation);
		delete(m_FlagNode);
	}
}