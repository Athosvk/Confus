#include <IrrAssimp/IrrAssimp.h>
#include <RakNet/BitStream.h>
#include <RakNet/GetTime.h>
#include <RakNet/PacketPriority.h>

#include "Player.h"
#include "Audio\PlayerAudioEmitter.h"
#include "Networking/ClientConnection.h"
#include "EventManager.h"
#include "Flag.h"
#include "../Confusshared/Physics/PhysicsWorld.h"
#include "../Confusshared/Physics/BoxCollider.h"
#include "../Confusshared/Physics/RigidBody.h"

namespace Confus
{
    const irr::u32 Player::WeaponJointIndex = 14u;
    const unsigned Player::LightAttackDamage = 10u;
    const unsigned Player::HeavyAttackDamage = 30u;


    Player::Player(irr::IrrlichtDevice* a_Device, Physics::PhysicsWorld& a_PhysicsWorld, char a_ID, ETeamIdentifier a_TeamIdentifier, bool a_MainPlayer, Audio::AudioManager* a_AudioManager)
        : m_Weapon(a_Device->getSceneManager(), a_PhysicsWorld, irr::core::vector3df(0.3f, 0.3f, 0.9f)),
        irr::scene::ISceneNode(nullptr, a_Device->getSceneManager(), -1),
        TeamIdentifier(a_TeamIdentifier),
        CarryingFlag(EFlagEnum::None)
    {
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

        if (a_TeamIdentifier == ETeamIdentifier::TeamBlue) {
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
            CameraNode->setPosition(irr::core::vector3df(0.f, 10.f, 11.f));
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

        m_SoundEmitter = new Audio::PlayerAudioEmitter(this, a_AudioManager);

        m_Weapon.setParent(PlayerNode->getJointNode(WeaponJointIndex));
        m_Weapon.disableCollider();
		PlayerNode->setAnimationEndCallback(this);
    }

	Player::~Player() {
        delete(m_SoundEmitter);
	}

    void Player::handleInput(EventManager& a_EventManager)
    {
		auto movementDirection = irr::core::vector3df();
		if(a_EventManager.IsKeyDown(irr::KEY_KEY_W))
		{
			movementDirection.Z = 1.0f;
		}
		else if(a_EventManager.IsKeyDown(irr::KEY_KEY_S))
		{
			movementDirection.Z = -1.0f;
		}
		if(a_EventManager.IsKeyDown(irr::KEY_KEY_A))
		{
			movementDirection.X = -1.0f;
		}
		else if (a_EventManager.IsKeyDown(irr::KEY_KEY_D))
		{
			movementDirection.X = 1.0f;
		}
		//Rotate with the negative xz-rotation (around the Y axis), as
		//the scene node convention seems to be clockwise while the rotate function
		//is counter-clockwise
		movementDirection.rotateXZBy(-CameraNode->getRotation().Y);
		movementDirection = movementDirection.normalize();
		auto rigidBody = m_Collider->getRigidBody();
		const float Speed = 15.0f;
		auto resultingVelocity = irr::core::vector3df(movementDirection.X, 0.0f, movementDirection.Z) * Speed
			+ irr::core::vector3df(0.0f, rigidBody->getVelocity().Y, 0.0f);
		//rigidBody->setVelocity(resultingVelocity);
    }

    void Player::render()
    {

    }

    const irr::core::aabbox3d<irr::f32>& Player::getBoundingBox() const
    {
        return m_Mesh->getBoundingBox();
    }

    void Player::updateFromServer(ConfusShared::Networking::PlayerInfo a_PlayerInfo)
    {
        CameraNode->setPosition(a_PlayerInfo.position);
        changeState(a_PlayerInfo.newState);

        if(MainPlayer == false)
        {
            CameraNode->setRotation(a_PlayerInfo.rotation);
        }
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
        PlayerNode->setFrameLoop(38, 41);
        PlayerNode->setCurrentFrame(38);
        m_Weapon.Damage = LightAttackDamage;
        m_SoundEmitter->playAttackSound(false);
        initializeAttack();
    }

    void Player::startHeavyAttack()
    {
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

        if(PlayerHealth.getHealth() <= 0) {
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
        updateServer();
    }

    void Player::fixedUpdate()
    {
    }

    void Player::respawn()
    {
		PlayerHealth.reset();
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

    void Player::setConnection(Networking::ClientConnection * a_Connection)
    {
        m_Connection = a_Connection;
    }

    void Player::updateServer() const
	{
		ConfusShared::Networking::PlayerInfo playerInfo;
		playerInfo.playerID = m_PlayerID;
		playerInfo.rotation = CameraNode->getRotation();
		playerInfo.stateChangeTime = m_StateChangeTime;
		playerInfo.forwardKeyPressed = m_EventManager->IsKeyDown(irr::KEY_KEY_W);
		playerInfo.backwardKeyPressed = m_EventManager->IsKeyDown(irr::KEY_KEY_S);
		playerInfo.leftKeyPressed = m_EventManager->IsKeyDown(irr::KEY_KEY_A);
		playerInfo.rightKeyPressed = m_EventManager->IsKeyDown(irr::KEY_KEY_D);
		playerInfo.leftMouseButtonPressed = m_EventManager->IsLeftMouseDown();
		playerInfo.rightMouseButtonPressed = m_EventManager->IsRightMouseDown();

        RakNet::BitStream bitstreamOut;
        bitstreamOut.Write(static_cast<RakNet::MessageID>(Networking::EPacketType::Player));
		bitstreamOut.Write(playerInfo);

        m_Connection->sendMessage(&bitstreamOut, PacketReliability::UNRELIABLE);
	}

     void Player::changeState(EPlayerState a_NewState)
     {
         // Only change state is it is different from the player's current state.
         if(a_NewState != m_PlayerState) 
         {
             m_PlayerState = a_NewState;

             if(m_PlayerState == EPlayerState::LightAttacking)
             {
                 startLightAttack();
             } 
             else if(m_PlayerState == EPlayerState::HeavyAttacking)
             {
                 startHeavyAttack();
             }

             m_StateChangeTime = RakNet::GetTime();
         } 
     }

     void Player::setEventManager(EventManager* a_Manager) 
     {
         m_EventManager = a_Manager;
     }
}
