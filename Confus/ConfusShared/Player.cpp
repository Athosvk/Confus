#include <iostream>
#include <string>

#include "EventManager.h"
#include "Flag.h"
#include "Physics/PhysicsWorld.h"
#include "Physics/BoxCollider.h"
#include "Physics/RigidBody.h"
#include "Player.h"

namespace Confus
{
    const irr::u32 Player::WeaponJointIndex = 14u;
    const unsigned Player::LightAttackDamage = 25u;
    const unsigned Player::HeavyAttackDamage = 50u;

	Player::Player(irr::IrrlichtDevice* a_Device, ConfusShared::Physics::PhysicsWorld& a_PhysicsWorld,
		long long a_ID)
		: irr::scene::ISceneNode(nullptr, a_Device->getSceneManager(), -1),
		m_Weapon(a_Device->getSceneManager(), a_PhysicsWorld, irr::core::vector3df(0.3f, 0.3f, 0.9f)),
		CarryingFlag(ConfusShared::EFlagEnum::None)
    {
        auto sceneManager = a_Device->getSceneManager();
        auto videoDriver = a_Device->getVideoDriver();

        m_Mesh = sceneManager->getMesh("Media/ninja.b3d");
        ID = a_ID;

		m_PlayerNode = sceneManager->addAnimatedMeshSceneNode(m_Mesh, this);
		m_PlayerNode->setMaterialFlag(irr::video::E_MATERIAL_FLAG::EMF_LIGHTING, false);
		m_PlayerNode->setScale(irr::core::vector3df(0.3f, 0.3f, 0.3f));
		m_PlayerNode->setPosition(irr::core::vector3df(0.f, -2.0f, -0.2f));
		m_PlayerNode->setName({"Player"});

		m_Collider = a_PhysicsWorld.createBoxCollider(irr::core::vector3df(0.6f, 2.7f, 0.6f), this,
			ConfusShared::Physics::ECollisionFilter::Player, ~ConfusShared::Physics::ECollisionFilter::Player);
		m_Collider->getRigidBody()->disableSleeping();
		m_Collider->getRigidBody()->setOffset(irr::core::vector3df(0.0f, -0.65f, -0.2f));
        startWalking();

        m_Weapon.setParent(m_PlayerNode->getJointNode(WeaponJointIndex));
        m_Weapon.disableCollider();
		m_PlayerNode->setAnimationEndCallback(this);
    }

	ConfusShared::Health* Player::getHealthInstance()
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
		m_PlayerNode->setAnimationEndCallback(nullptr);
		m_PlayerNode->setLoopMode(true);
		m_PlayerNode->setFrameLoop(0, 13);
		m_PlayerNode->setCurrentFrame(7);
		m_PlayerNode->setAnimationSpeed(24);
    }

    void Player::stopWalking() const
    {
        m_PlayerNode->setAnimationEndCallback(nullptr);
        m_PlayerNode->setLoopMode(false);
        m_PlayerNode->setCurrentFrame(7);
        m_PlayerNode->setAnimationSpeed(0);
    }

    void Player::initializeAttack()
    {
        m_PlayerNode->setLoopMode(false);
        m_PlayerNode->setAnimationEndCallback(this);
        m_PlayerNode->setAnimationSpeed(10);
        m_Attacking = true;
        m_Weapon.enableCollider();
        m_Weapon.resetCollider();
    }

    void Player::startLightAttack()
    {
        changeState(EPlayerState::LightAttacking);
        m_PlayerNode->setFrameLoop(38, 41);
        m_PlayerNode->setCurrentFrame(38);
        m_Weapon.Damage = LightAttackDamage;
		OnLightAttack();
        initializeAttack();
    }

    void Player::startHeavyAttack()
    {
        changeState(EPlayerState::HeavyAttacking);
        m_PlayerNode->setFrameLoop(60, 66);
        m_PlayerNode->setCurrentFrame(60);
        m_Weapon.Damage = HeavyAttackDamage;
		OnHeavyAttack();
        initializeAttack();
    }

	EPlayerState Player::getState() const
	{
		return m_PlayerState;
	}

	void Player::setStartPosition(irr::core::vector3df a_Position)
	{
		m_StartPosition = a_Position;
	}

	ConfusShared::ETeamIdentifier Player::getTeamIdentifier() const
	{
		return m_TeamIdentifier;
	}

	void Player::setTeamIdentifier(ConfusShared::ETeamIdentifier a_TeamIdentifier, irr::IrrlichtDevice* a_Device)
	{
		m_TeamIdentifier = a_TeamIdentifier;
		updateColor(a_Device);
	}

	int Player::getAnimationFrame() const
	{
		return static_cast<int>(m_PlayerNode->getFrameNr());
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
        if(m_TeamIdentifier == ConfusShared::ETeamIdentifier::TeamBlue) 
		{
			m_PlayerNode->setMaterialTexture(0, videoDriver->getTexture("Media/nskinbl.jpg"));
        }
        else if(m_TeamIdentifier == ConfusShared::ETeamIdentifier::TeamRed) 
		{
			m_PlayerNode->setMaterialTexture(0, videoDriver->getTexture("Media/nskinrd.jpg"));
        }
    }

    void Player::update()
    {
        if(m_PlayerHealth.getHealth() <= 0) 
		{
            respawn();
			if (FlagPointer != nullptr) 
			{
				dropFlag();
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

        if(getPosition().Y <= -10) 
		{
            respawn();
			if (FlagPointer != nullptr) 
			{
				FlagPointer->returnToStartPosition();
			}
        }
    }

    void Player::fixedUpdate()
    {
    }

    void Player::respawn()
    {
		m_PlayerHealth.reset();
		setPosition(m_StartPosition);
    }

    void Player::changeState(EPlayerState a_NewState)
    {
        // Only change state if it is different from the player's current state.
        if(a_NewState != m_PlayerState) 
        {
            m_PlayerState = a_NewState;
        } 
    }

	void Player::onScore()
	{
		FlagPointer = nullptr;
		CarryingFlag = ConfusShared::EFlagEnum::None;
	}

	void Player::dropFlag()
	{
		FlagPointer->drop(getAbsolutePosition());
		FlagPointer = nullptr;
		CarryingFlag = ConfusShared::EFlagEnum::None;
	}
}
