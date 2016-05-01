#include <IrrAssimp/IrrAssimp.h>

#include "Audio\PlayerAudioEmitter.h"
#include "Player.h"
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

	Player::Player(irr::IrrlichtDevice* a_Device, Physics::PhysicsWorld& a_PhysicsWorld, irr::s32 a_ID, ETeamIdentifier a_TeamIdentifier, bool a_MainPlayer)
		: m_Weapon(a_Device->getSceneManager(), a_PhysicsWorld, irr::core::vector3df(0.3f, 0.3f, 0.9f)),
		irr::scene::ISceneNode(nullptr, a_Device->getSceneManager(), a_ID),
		TeamIdentifier(new ETeamIdentifier(a_TeamIdentifier)),
		CarryingFlag(new EFlagEnum(EFlagEnum::None))
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

        if(a_TeamIdentifier == ETeamIdentifier::TeamBlue) {
            PlayerNode->setMaterialTexture(0, videoDriver->getTexture("Media/nskinbl.jpg"));
        }
        else if(a_TeamIdentifier == ETeamIdentifier::TeamRed) {
            PlayerNode->setMaterialTexture(0, videoDriver->getTexture("Media/nskinrd.jpg"));
        }

        m_KeyMap[0].Action = irr::EKA_MOVE_FORWARD;
        m_KeyMap[0].KeyCode = irr::KEY_KEY_0;

        m_KeyMap[1].Action = irr::EKA_MOVE_BACKWARD;
        m_KeyMap[1].KeyCode = irr::KEY_KEY_0;

        m_KeyMap[2].Action = irr::EKA_STRAFE_LEFT;
        m_KeyMap[2].KeyCode = irr::KEY_KEY_0;

        m_KeyMap[3].Action = irr::EKA_STRAFE_RIGHT;
        m_KeyMap[3].KeyCode = irr::KEY_KEY_0;

        m_KeyMap[4].Action = irr::EKA_JUMP_UP;
        m_KeyMap[4].KeyCode = irr::KEY_KEY_0;

        if(a_MainPlayer) 
        {
            CameraNode = sceneManager->addCameraSceneNodeFPS(0, 100.0f, 0.01f, 1, m_KeyMap, 5, true, 0.5f, false, true);
            CameraNode->setFOV(70.f);
            CameraNode->setNearValue(0.1f);
        }
        else 
        {
            CameraNode = sceneManager->addCameraSceneNodeFPS(0, 100.0f, 0.01f, 1, m_KeyMap, 5, true, 0.5f, false, false);
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
		m_Collider = a_PhysicsWorld.createBoxCollider(irr::core::vector3df(0.5f, 2.3f, 0.5f), CameraNode,
			Physics::ECollisionFilter::Player, ~Physics::ECollisionFilter::Player);
		m_Collider->getRigidBody()->disableSleeping();

        createAudioEmitter();
        startWalking();

        m_Weapon.setParent(PlayerNode->getJointNode(WeaponJointIndex));
        m_Weapon.disableCollider();
		PlayerNode->setAnimationEndCallback(this);
    }

	Player::~Player() {
		delete(CarryingFlag);
		delete(TeamIdentifier);
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
		static float timer = 5.0f;
		if(a_EventManager.IsKeyDown(irr::KEY_SPACE) && timer >= 5.0f)
		{
			timer = 0.0f;
			rigidBody->applyForce(irr::core::vector3df(0.0f, 1200.0f, 0.0f));
		}
		timer += 0.20f;
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

        if(CameraNode->getPosition().Y <= -10) {
            respawn();
			if (FlagPointer != nullptr) {
				FlagPointer->returnToStartPosition();
			}
        }
    }

    void Player::respawn()
    {
        if(*TeamIdentifier == ETeamIdentifier::TeamBlue)
        {
             CameraNode->setPosition(irr::core::vector3df(0.f, 10.f, 11.f));
        }
        else if(*TeamIdentifier == ETeamIdentifier::TeamRed)
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

    void Player::createAudioEmitter()
    {
        m_SoundEmitter = new Audio::PlayerAudioEmitter(PlayerNode);
    }
}
