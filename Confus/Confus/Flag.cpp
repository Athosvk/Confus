#include <irrlicht/irrlicht.h>
#include <IrrAssimp/IrrAssimp.h>

#include "Flag.h"
#include "Player.h"


namespace Confus {

	Flag::Flag(irr::IrrlichtDevice* a_Device, TeamIdentifier a_TeamIdentifier) : m_TeamIdentifier(a_TeamIdentifier) {
        //Get drivers to load model
        auto sceneManager = a_Device->getSceneManager();
        auto videoDriver = a_Device->getVideoDriver();

        //Load model
        IrrAssimp irrAssimp(sceneManager);
        irr::scene::IAnimatedMesh* mesh = sceneManager->getMesh("Media/Models/flag.obj");
        m_FlagNode = sceneManager->addAnimatedMeshSceneNode(mesh);
        m_FlagNode->setMaterialFlag(irr::video::E_MATERIAL_FLAG::EMF_LIGHTING, false);
        m_FlagNode->setScale({ 0.03f, 0.03f, 0.03f });
        
        //Set color & position based on color
        switch(m_TeamIdentifier)
        {
        case TeamIdentifier::TEAM_BLUE:
            m_FlagNode->setMaterialTexture(0, videoDriver->getTexture("Media/Textures/Flag/FLAG_BLUE.png"));
            m_StartPosition = { -2.5f, 3.5f, -5.f };
            m_StartRotation = { 0.f, 0.f, 0.f };
            Return();
            break;
        case TeamIdentifier::TEAM_RED:
            m_FlagNode->setMaterialTexture(0, videoDriver->getTexture("Media/Textures/Flag/FLAG_RED.png"));
            m_StartPosition = { 3.5f, 3.5f, -75.f };
            m_StartRotation = { 0.f, 180.f, 0.f };
            Return();
            break;
        default:
            m_StartPosition = { 0, 0, 0 };
            m_StartRotation = { 0, 0, 0 };
            break;
        }      

        //Add colission box and particle system
        auto collisionBox = sceneManager->addCubeSceneNode(1.0f, m_FlagNode, 0, irr::core::vector3d<float>(0.0f, 5.0f, 0.0f), irr::core::vector3d<float>(0.0f, 0.0f, 0.0f), irr::core::vector3d<float>(2.5f, 10.0f, 2.0f));
        collisionBox->setVisible(false);
        InitParticleSystem(sceneManager);
	}

    void Flag::InitParticleSystem(irr::scene::ISceneManager* a_SceneManager) {
        //Create Particle System
        irr::scene::IParticleSystemSceneNode* ps = a_SceneManager->addParticleSystemSceneNode(false);
        auto colDark = GetColor();
        auto colBright = colDark;
        colBright.setAlpha(0);

        //Emitter
        irr::scene::IParticleEmitter* em = ps->createBoxEmitter(
            irr::core::aabbox3d<irr::f32>(-7, 0, -7, 7, 1, 7),      // emitter size
            irr::core::vector3df(0.0f, 0.06f, 0.0f),                // initial direction
            40, 50,                                                 // emit rate
            colDark, colBright,                                     // dark en bright color
            400, 1000, 0,                                           // min and max age, angle
            { 0.5f, 0.5f }, { 1.f, 1.f });                          //min and max size

        //Affector
        irr::scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();
        ps->setScale({ 2, 2, 2 });
        ps->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        ps->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
        ps->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
        ps->setParent(m_FlagNode);
        ps->setPosition({ 0, 2.5f, 0 });

        //Drop systems after setting them
        ps->setEmitter(em);
        em->drop();
        ps->addAffector(paf);
        paf->drop();
    }

    irr::video::SColor Flag::GetColor() {
        switch(m_TeamIdentifier)
        {
        case TeamIdentifier::TEAM_BLUE:
            return { 255, 0, 0, 255 };
        case TeamIdentifier::TEAM_RED:
            return { 255, 255, 0, 0 };
        default:
            return { 255, 255, 255, 255 };
        }
    }

	//This class handles what to do on collision
	void Flag::CaptureFlag(TeamIdentifier a_PlayerTeamID, Player* a_PlayerObject) {
		if (m_FlagStatus == FlagEnum::FLAG_TAKEN)
			return;

		if (a_PlayerTeamID != m_TeamIdentifier) {
			//Capture Flag
		}
		else if (a_PlayerTeamID == m_TeamIdentifier) {
			//If flag has been dropped return flag to base
			if (m_FlagStatus == FlagEnum::FLAG_DROPPED) {
				Return();
			}
			//If flag is at base and player is carrying a flag
			else if (m_FlagStatus == FlagEnum::FLAG_BASE) {
				if (a_PlayerObject->m_CarryingFlag == FlagEnum::FLAG_TAKEN) {
					Score(a_PlayerObject);
				}
			}
		}
	}

	///Score points to team of a_PlayerObject
	void Flag::Score(Player* a_PlayerObject) {
		
	}

	///Drop Flag at position of player
	void Flag::Drop(Player* a_PlayerObject) {
		
	}

    void Flag::SetStartPosition(irr::core::vector3df a_Position) {
        m_StartPosition = a_Position;
    }

    void Flag::SetStartRotation(irr::core::vector3df a_Rotation) {
        m_StartRotation = a_Rotation;
    }

    void Flag::Return() {
        m_FlagNode->setPosition(m_StartPosition);
        m_FlagNode->setRotation(m_StartRotation);
    }

	Flag::~Flag() {

	}
}