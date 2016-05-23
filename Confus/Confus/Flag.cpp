#include <Irrlicht/irrlicht.h>
#include <IrrAssimp/IrrAssimp.h>
#include <iostream>

#include <RakNet/BitStream.h>
#include "Player.h"
#include "ClientTeamScore.h"

#include "Flag.h"

namespace Confus 
{
	Flag::Flag(irr::IrrlichtDevice* a_Device, ETeamIdentifier a_TeamIdentifier) : 
		m_TeamIdentifier(a_TeamIdentifier),
		m_FlagStatus(EFlagEnum::FlagBase) 
	{
        //Get drivers to load model
        auto sceneManager = a_Device->getSceneManager();
        auto videoDriver = a_Device->getVideoDriver();

        //Load model
        IrrAssimp irrAssimp(sceneManager);
        irr::scene::IAnimatedMesh* mesh = sceneManager->getMesh("Media/Meshes/Flag.3ds");
        m_FlagNode = sceneManager->addMeshSceneNode(mesh, 0, 2);
        m_FlagNode->setMaterialFlag(irr::video::E_MATERIAL_FLAG::EMF_LIGHTING, false);
        m_FlagNode->setScale({ 1.5f, 1.5f, 1.5f });
        m_FlagOldParent = m_FlagNode->getParent();

        //Set Color
		setColor(videoDriver);

        //Add colission box and particle system
        initParticleSystem(sceneManager);
	}

	//Set color & position based on color of flag
	void Flag::setColor(irr::video::IVideoDriver* a_VideoDriver) 
	{
		switch (m_TeamIdentifier)
		{
		case ETeamIdentifier::TeamBlue:
            m_FlagNode->setMaterialTexture(0, a_VideoDriver->getTexture("Media/Textures/Flag/FLAG_BLUE.png"));
			m_StartPosition.set({ -2.0f, 5.f, -2.f });
			m_StartRotation.set({ 0.f, 0.f, 0.f });
			returnToStartPosition();
			break;
		case ETeamIdentifier::TeamRed:
            m_FlagNode->setMaterialTexture(0, a_VideoDriver->getTexture("Media/Textures/Flag/FLAG_RED.png"));
			m_StartPosition.set({ 1.5f, 5.f, -72.f });
			m_StartRotation.set({ 0.f, 180.f, 0.f });
            returnToStartPosition();
			break;
		default:
			m_StartPosition.set({ 0, 0, 0 });
			m_StartRotation.set({ 0, 0, 0 });
			break;
		}
	}

	void Flag::setFlagStatus(EFlagEnum a_FlagStatus)
	{
 		FlagStatusChangedEvent(m_TeamIdentifier, m_FlagStatus, a_FlagStatus);
		m_FlagStatus = a_FlagStatus;
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
        switch(m_TeamIdentifier)
        {
        case ETeamIdentifier::TeamBlue:
            return { 255, 0, 0, 255 };
        case ETeamIdentifier::TeamRed:
            return { 255, 255, 0, 0 };
        default:
            return { 255, 255, 255, 255 };
        }
    }

	 const ETeamIdentifier Flag::getTeamIdentifier() const
	 {
		 return m_TeamIdentifier;
	 }

	 const EFlagEnum  Flag::getFlagStatus() const
	 {
		 return m_FlagStatus;
	 }

	//TODO Score points to team of a_PlayerObject
	void Flag::score(Player* a_PlayerObject) 
    {
		a_PlayerObject->FlagPointer->returnToStartPosition();
		a_PlayerObject->FlagPointer = nullptr;
        a_PlayerObject->CarryingFlag = EFlagEnum::None;
		 ClientTeamScore::setTeamScore(a_PlayerObject->TeamIdentifier, ClientTeamScore::getTeamScore(a_PlayerObject->TeamIdentifier) + 1);
	}

	void Flag::drop(Player* a_PlayerObject) 
    {
        m_FlagNode->setParent(m_FlagOldParent);
        m_FlagNode->setPosition(a_PlayerObject->PlayerNode->getAbsolutePosition());
        a_PlayerObject->FlagPointer = nullptr;
        a_PlayerObject->CarryingFlag = EFlagEnum::None;
		setFlagStatus(EFlagEnum::FlagDropped);
	}

    void Flag::setStartPosition(irr::core::vector3df a_Position) 
    {
        m_StartPosition.set(a_Position);
    }

    void Flag::setStartRotation(irr::core::vector3df a_Rotation) 
    {
        m_StartRotation.set(a_Rotation);
    }

    void Flag::setPosition(irr::core::vector3df a_Position)
    {
        m_FlagNode->setPosition(a_Position);
    }

    void Flag::returnToStartPosition() {
        m_FlagNode->setParent(m_FlagOldParent);
        m_FlagNode->setPosition(m_StartPosition);
        m_FlagNode->setRotation(m_StartRotation);
		setFlagStatus(EFlagEnum::FlagBase);
    }

    void Flag::setConnection(Networking::ClientConnection* a_Connection)
    {
        m_Connection = a_Connection;

         m_Connection->addFunctionToMap(static_cast<unsigned char>(Networking::EPacketType::Flag), [this](RakNet::Packet* a_Packet)
        {
            RakNet::BitStream inputStream(a_Packet->data, a_Packet->length, false);
            
            ETeamIdentifier teamIdentifier;
            inputStream.IgnoreBytes(sizeof(unsigned char));
            inputStream.Read(teamIdentifier);

            // Only continue if the packet is for this flag color.
            if(teamIdentifier == m_TeamIdentifier)
            {
                EFlagEnum flagStatus;
                irr::core::vector3df flagPosition;
                inputStream.Read(flagStatus);
                inputStream.Read(flagPosition);
                
                setFlagStatus(flagStatus);
                setPosition(flagPosition);

                std::cout << std::to_string(flagPosition.Y);
            }
        });
    }

	Flag::~Flag() {
	}
}
