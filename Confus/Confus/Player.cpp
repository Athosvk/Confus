#include <IrrAssimp/IrrAssimp.h>

#include "Audio\PlayerAudioEmitter.h"
#include "Player.h"
#include "../ConfusShared/EventManager.h"
#include "../ConfusShared/Flag.h"
#include "../Confusshared/Physics/PhysicsWorld.h"
#include "../Confusshared/Physics/BoxCollider.h"
#include "../Confusshared/Physics/RigidBody.h"

namespace Confus
{
    const irr::u32 Player::WeaponJointIndex = 14u;
    const unsigned Player::LightAttackDamage = 10u;
    const unsigned Player::HeavyAttackDamage = 30u;

    Player::Player(irr::IrrlichtDevice* a_Device, ConfusShared::Physics::PhysicsWorld& a_PhysicsWorld, 
		long long a_ID, ConfusShared::ETeamIdentifier a_TeamIdentifier, bool a_MainPlayer, 
		Confus::Audio::AudioManager* a_AudioManager)
		: m_Weapon(a_Device->getSceneManager(), a_PhysicsWorld, irr::core::vector3df(0.3f, 0.3f, 0.9f)),
        irr::scene::ISceneNode(nullptr, a_Device->getSceneManager(), -1),
        TeamIdentifier(a_TeamIdentifier),
        CarryingFlag(ConfusShared::EFlagEnum::None)
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

        if(a_TeamIdentifier == ConfusShared::ETeamIdentifier::TeamBlue) {
            PlayerNode->setMaterialTexture(0, videoDriver->getTexture("Media/nskinbl.jpg"));
        }
        else if(a_TeamIdentifier == ConfusShared::ETeamIdentifier::TeamRed) {
            PlayerNode->setMaterialTexture(0, videoDriver->getTexture("Media/nskinrd.jpg"));
        }

        if(a_MainPlayer) 
        {
            CameraNode = sceneManager->addCameraSceneNodeFPS(0, 100.0f, 0.0f, 1, nullptr, 0, true, 0.0f);
            CameraNode->setFOV(70.f);
            CameraNode->setNearValue(0.1f);
        }
        else 
        {
            CameraNode = sceneManager->addCameraSceneNodeFPS(0, 100.0f, 0.0f, 1, nullptr, 0, true, 0.0f, false, false);
        }
        if(a_TeamIdentifier == ConfusShared::ETeamIdentifier::TeamBlue)
        {
            CameraNode->setPosition(irr::core::vector3df(0.f, 10.f, 0.f));
        }
        else if(a_TeamIdentifier == ConfusShared::ETeamIdentifier::TeamRed)
        {
            CameraNode->setPosition(irr::core::vector3df(0.f, 10.f, -85.f));
        }
	    PlayerNode->setParent(this);
		setParent(CameraNode);
		m_Collider = a_PhysicsWorld.createBoxCollider(irr::core::vector3df(0.6f, 2.7f, 0.6f), CameraNode,
			ConfusShared::Physics::ECollisionFilter::Player, ~ConfusShared::Physics::ECollisionFilter::Player);
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

    void Player::handleInput(ConfusShared::EventManager& a_EventManager)
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
		else if(a_EventManager.IsKeyDown(irr::KEY_KEY_D))
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
		rigidBody->setVelocity(resultingVelocity);
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
        if(TeamIdentifier == ConfusShared::ETeamIdentifier::TeamBlue) {
            PlayerNode->setMaterialTexture(0, videoDriver->getTexture("Media/nskinbl.jpg"));
        }
        else if(TeamIdentifier == ConfusShared::ETeamIdentifier::TeamRed) {
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
    }

    void Player::respawn()
    {
		PlayerHealth.reset();
        if(TeamIdentifier == ConfusShared::ETeamIdentifier::TeamBlue)
        {
            CameraNode->setPosition(irr::core::vector3df(0.f, 10.f, 11.f));
        }
        else if(TeamIdentifier == ConfusShared::ETeamIdentifier::TeamRed)
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
}
