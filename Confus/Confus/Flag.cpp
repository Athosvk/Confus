#include <irrlicht/irrlicht.h>
#include <IrrAssimp/IrrAssimp.h>

#include "Flag.h"
#include "Player.h"


namespace Confus {

	Flag::Flag(irr::IrrlichtDevice* a_Device, TeamIdentifier a_TeamIdentifier) : m_TeamIdentifier(a_TeamIdentifier) {
        auto sceneManager = a_Device->getSceneManager();
        auto videoDriver = a_Device->getVideoDriver();

        IrrAssimp irrAssimp(sceneManager);
        irr::scene::IAnimatedMesh* mesh = sceneManager->getMesh("Media/flag.obj");
        m_FlagNode = sceneManager->addAnimatedMeshSceneNode(mesh);
        m_FlagNode->setMaterialFlag(irr::video::E_MATERIAL_FLAG::EMF_LIGHTING, false);

        switch(m_TeamIdentifier)
        {
        case TeamIdentifier::TEAM_BLUE:
            m_FlagNode->setMaterialTexture(0, videoDriver->getTexture("Media/Textures/Flag/FLAG_BLUE.png"));
            m_StartPosition = irr::core::vector3df(-2.5f, 3.5f, -5.f);
            m_StartRotation = irr::core::vector3df(0.f, 0.f, 0.f);
            Return();
            break;
        case TeamIdentifier::TEAM_RED:
            m_FlagNode->setMaterialTexture(0, videoDriver->getTexture("Media/Textures/Flag/FLAG_RED.png"));
            m_StartPosition = irr::core::vector3df(3.5f, 3.5f, -75.f);
            m_StartRotation = irr::core::vector3df(0.f, 180.f, 0.f);
            Return();
            break;
        default:
            m_StartPosition = irr::core::vector3df(0, 0, 0);
            m_StartRotation = irr::core::vector3df(0, 0, 0);
            break;
        }
        

        m_FlagNode->setScale(irr::core::vector3df(0.03f, 0.03f, 0.03f));

        auto collisionBox = sceneManager->addCubeSceneNode(1.0f, m_FlagNode, 0, irr::core::vector3d<float>(0.0f, 5.0f, 0.0f), irr::core::vector3d<float>(0.0f, 0.0f, 0.0f), irr::core::vector3d<float>(2.5f, 10.0f, 2.0f));
        collisionBox->setVisible(false);
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