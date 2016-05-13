#include <IrrAssimp/IrrAssimp.h>
#include <iostream>
#include <string>
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/BitStream.h>
#include <RakNet/GetTime.h>
#include <RakNet/RakPeer.h>
#include <RakNet/RakNetTypes.h>
#include "Audio\PlayerAudioEmitter.h"
#include "Networking/ClientConnection.h"
#include "Player.h"
#include "EventManager.h"
#include "Flag.h"
#include <RakNet/PacketPriority.h>
#include "../Confusshared/Physics/PhysicsWorld.h"
#include "../Confusshared/Physics/BoxCollider.h"
#include "../Confusshared/Physics/RigidBody.h"

namespace Confus
{
    Networking::ClientConnection* m_Connection;
    const irr::u32 Player::WeaponJointIndex = 14u;
    const unsigned Player::LightAttackDamage = 25u;
    const unsigned Player::HeavyAttackDamage = 50u;


	Player::Player(irr::IrrlichtDevice* a_Device, Physics::PhysicsWorld& a_PhysicsWorld, long long a_ID, ETeamIdentifier a_TeamIdentifier, bool a_MainPlayer, Confus::Audio::AudioManager* a_AudioManager)
		: m_Weapon(a_Device->getSceneManager(), a_PhysicsWorld, irr::core::vector3df(0.3f, 0.3f, 0.9f)),
		irr::scene::ISceneNode(nullptr, a_Device->getSceneManager(), -1),
		TeamIdentifier(a_TeamIdentifier),
		CarryingFlag(EFlagEnum::None),
		m_SoundEmitter(new Audio::PlayerAudioEmitter(this, a_AudioManager)),
		m_PlayerHealth()
    {
		auto damageEvents = [this](EHitIdentifier a_HitIdentifier) -> void
		{
			m_SoundEmitter->playHitSound(a_HitIdentifier);
		};
		m_PlayerHealth.DamageEvent += damageEvents;

        auto sceneManager = a_Device->getSceneManager();
        auto videoDriver = a_Device->getVideoDriver();

        IrrAssimp irrAssimp(sceneManager);
        m_Mesh = sceneManager->getMesh("Media/ninja.b3d");
        MainPlayer = a_MainPlayer;
        ID = a_ID;

        PlayerNode = sceneManager->addAnimatedMeshSceneNode(m_Mesh, nullptr);
        PlayerNode->setMaterialFlag(irr::video::E_MATERIAL_FLAG::EMF_LIGHTING, false);
        PlayerNode->setScale(irr::core::vector3df(0.3f, 0.3f, 0.3f));
        PlayerNode->setPosition(irr::core::vector3df(0.f, -2.0f, -0.2f));
        PlayerNode->setName({"Player"});

        if(a_TeamIdentifier == ETeamIdentifier::TeamBlue) {
            PlayerNode->setMaterialTexture(0, videoDriver->getTexture("Media/nskinbl.jpg"));
        }
        else if(a_TeamIdentifier == ETeamIdentifier::TeamRed) {
            PlayerNode->setMaterialTexture(0, videoDriver->getTexture("Media/nskinrd.jpg"));
        }

        if(a_MainPlayer) 
        {
            CameraNode = sceneManager->addCameraSceneNodeFPS(0, 100.0f, 0.0f, 1, nullptr, 0, true, 0.0f);
            CameraNode->setFOV(70.f);
            CameraNode->setNearValue(0.1f);
            m_IsMainPlayer = true;
        }
        else 
        {
            CameraNode = sceneManager->addCameraSceneNodeFPS(0, 100.0f, 0.0f, 1, nullptr, 0, true, 0.0f, false, false);
        }
        if(a_TeamIdentifier == ETeamIdentifier::TeamBlue)
        {
            CameraNode->setPosition(irr::core::vector3df(0.f, 10.f, 0.f));
        }
        else if(a_TeamIdentifier == ETeamIdentifier::TeamRed)
        {
            CameraNode->setPosition(irr::core::vector3df(0.f, 10.f, -85.f));
        }
	    PlayerNode->setParent(this);
        
		setParent(CameraNode);
		m_Collider = a_PhysicsWorld.createBoxCollider(irr::core::vector3df(0.6f, 2.7f, 0.6f), CameraNode,
			Physics::ECollisionFilter::Player, ~Physics::ECollisionFilter::Player);
		m_Collider->getRigidBody()->disableSleeping();
		m_Collider->getRigidBody()->setOffset(irr::core::vector3df(0.0f, -0.65f, -0.2f));
        startWalking();

        m_Weapon.setParent(PlayerNode->getJointNode(WeaponJointIndex));
        m_Weapon.disableCollider();
		PlayerNode->setAnimationEndCallback(this);
    }

	Player::~Player() {
        delete(m_SoundEmitter);
	}

    void Player::handleInput(EventManager& a_EventManager)
    {
        if(!m_Attacking)
        {
            if(a_EventManager.IsRightMouseDown())
            {
                startHeavyAttack();
            }
            else if(a_EventManager.IsLeftMouseDown())
            {
                startLightAttack();
            }
        }

    }

	Health* Player::getHealthInstance()
	{
		return &m_PlayerHealth;
	}

    void Player::render()
    {
       
    }

    const irr::core::aabbox3d<irr::f32>& Player::getBoundingBox() const
    {
        return m_Mesh->getBoundingBox();
    }

    void Player::startWalking() const
    {
        PlayerNode->setAnimationEndCallback(nullptr);
        PlayerNode->setLoopMode(true);
        PlayerNode->setFrameLoop(0, 13);
        PlayerNode->setCurrentFrame(7);
        PlayerNode->setAnimationSpeed(24);
    }

    void Player::stopWalking() const
    {
        PlayerNode->setAnimationEndCallback(nullptr);
        PlayerNode->setLoopMode(false);
        PlayerNode->setCurrentFrame(7);
        PlayerNode->setAnimationSpeed(0);
    }

    void Player::initializeAttack()
    {
        PlayerNode->setLoopMode(false);
        PlayerNode->setAnimationEndCallback(this);
        PlayerNode->setAnimationSpeed(10);
        m_Attacking = true;
        m_Weapon.enableCollider();
        m_Weapon.resetCollider();
    }

    void Player::startLightAttack()
    {
        changeState(EPlayerState::LightAttacking);
        PlayerNode->setFrameLoop(38, 41);
        PlayerNode->setCurrentFrame(38);
        m_Weapon.Damage = LightAttackDamage;
        m_SoundEmitter->playAttackSound(false);
        initializeAttack();

    }

    void Player::startHeavyAttack()
    {
        changeState(EPlayerState::HeavyAttacking);
        PlayerNode->setFrameLoop(60, 66);
        PlayerNode->setCurrentFrame(60);
        m_Weapon.Damage = HeavyAttackDamage;
        m_SoundEmitter->playAttackSound(true);
        initializeAttack();
    }

    void Player::OnAnimationEnd(irr::scene::IAnimatedMeshSceneNode* node)
    {
        if(m_Attacking)
        {
            m_Attacking = false;
            m_Weapon.disableCollider();
            startWalking();
            // Reset the player state.
            changeState(EPlayerState::Alive);
        }
    }

    void Player::updateColor(irr::IrrlichtDevice* a_Device)
    {
        auto videoDriver = a_Device->getVideoDriver();
        if(TeamIdentifier == ETeamIdentifier::TeamBlue) {
            PlayerNode->setMaterialTexture(0, videoDriver->getTexture("Media/nskinbl.jpg"));
        }
        else if(TeamIdentifier == ETeamIdentifier::TeamRed) {
            PlayerNode->setMaterialTexture(0, videoDriver->getTexture("Media/nskinrd.jpg"));
        }
    }

    void Player::update()
    {
        m_SoundEmitter->updatePosition();

        int frameNumber = static_cast<int>(PlayerNode->getFrameNr());
        if(frameNumber == 0 || frameNumber == 6)
        {
            m_SoundEmitter->playFootStepSound();
        }

        if(m_PlayerHealth.getHealth() <= 0) {
            respawn();
			if (FlagPointer != nullptr) {
				FlagPointer->drop(this);
			}
        }
        if(m_Collider->getRigidBody()->getVelocity().X != 0.0f && m_Collider->getRigidBody()->getVelocity().Z != 0.0f)
        {
            if(!m_Attacking && !m_Walking)
            {
                startWalking();
                m_Walking = true;
            }
        }
        else if(m_Walking && !m_Attacking)
        {
            stopWalking();
            m_Walking = false;
        }

        if(CameraNode->getPosition().Y <= -10) {
            respawn();
			if (FlagPointer != nullptr) {
				FlagPointer->returnToStartPosition();
			}
        }
    }

    void Player::fixedUpdate()
    {
        updateServer();
    }

    void Player::respawn()
    {
		m_PlayerHealth.reset();
        if(TeamIdentifier == ETeamIdentifier::TeamBlue)
        {
            CameraNode->setPosition(irr::core::vector3df(0.f, 10.f, 11.f));
        }
        else if(TeamIdentifier == ETeamIdentifier::TeamRed)
        {
            CameraNode->setPosition(irr::core::vector3df(0.f, 10.f, -85.f));
        }
    }

    void Player::updatePosition(irr::core::vector3df a_NewPosition)
    {
        CameraNode->setPosition(a_NewPosition);
    }

    void Player::updateRotation(irr::core::vector3df a_NewRotation)
    {
        CameraNode->setRotation(a_NewRotation);
    }


     void Player::setConnection(Networking::ClientConnection* a_Connection)
	{
        m_Connection = a_Connection;

        m_PlayerID = m_Connection->m_Interface->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS);

        m_Connection->addFunctionToMap(static_cast<unsigned char>(Networking::EPacketType::Player), [this](RakNet::Packet* a_Data)
        {
            RakNet::BitStream bitstreamIn(a_Data->data, a_Data->length, false);

            irr::core::vector3df newPosition;

            bitstreamIn.IgnoreBytes(sizeof(unsigned char));
            bitstreamIn.Read(newPosition);

            CameraNode->setPosition(newPosition);
        });
	}

     void Player::updateServer() const
	{
        RakNet::BitStream bitstreamOut;

        bitstreamOut.Write(static_cast<RakNet::MessageID>(Networking::EPacketType::Player));
        bitstreamOut.Write(m_PlayerID);
        bitstreamOut.Write(CameraNode->getPosition());
        bitstreamOut.Write(CameraNode->getRotation());
        bitstreamOut.Write(Player::m_PlayerState);
        bitstreamOut.Write(m_StateChangeTime);
        bitstreamOut.Write(m_PlayerHealthPoints);
        bitstreamOut.Write(m_EventManager->IsKeyDown(irr::KEY_KEY_W));
        bitstreamOut.Write(m_EventManager->IsKeyDown(irr::KEY_KEY_S));
        bitstreamOut.Write(m_EventManager->IsKeyDown(irr::KEY_KEY_A));
        bitstreamOut.Write(m_EventManager->IsKeyDown(irr::KEY_KEY_D));

        m_Connection->sendMessage(&bitstreamOut, PacketReliability::UNRELIABLE);
	}

     void Player::changeState(EPlayerState a_NewState)
     {
         // Only change state is it is different from the player's current state.
         if(a_NewState != m_PlayerState) 
         {
             m_PlayerState = a_NewState;
             m_StateChangeTime = RakNet::GetTime();
             std::cout << "State changed! Time of change is: " << m_StateChangeTime;
         } 
     }

     void Player::setEventManager(EventManager* a_Manager) 
     {
         m_EventManager = a_Manager;
     }
}
