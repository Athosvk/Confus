#include <IrrAssimp/IrrAssimp.h>
#include "Audio\PlayerAudioEmitter.h"
#include "Player.h"
#include "EventManager.h"
#include "Flag.h"

namespace Confus
{
    const irr::u32 Player::WeaponJointIndex = 14u;
    const unsigned Player::LightAttackDamage = 10u;
    const unsigned Player::HeavyAttackDamage = 30u;
	Player::Player(irr::IrrlichtDevice* a_Device, irr::s32 a_ID, ETeamIdentifier a_TeamIdentifier, bool a_MainPlayer)
		: m_Weapon(a_Device->getSceneManager(), irr::core::vector3df(1.0f, 1.0f, 4.0f)),
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
        m_KeyMap[0].KeyCode = irr::KEY_KEY_W;

        m_KeyMap[1].Action = irr::EKA_MOVE_BACKWARD;
        m_KeyMap[1].KeyCode = irr::KEY_KEY_S;

        m_KeyMap[2].Action = irr::EKA_STRAFE_LEFT;
        m_KeyMap[2].KeyCode = irr::KEY_KEY_A;

        m_KeyMap[3].Action = irr::EKA_STRAFE_RIGHT;
        m_KeyMap[3].KeyCode = irr::KEY_KEY_D;

        m_KeyMap[4].Action = irr::EKA_JUMP_UP;
        m_KeyMap[4].KeyCode = irr::KEY_SPACE;

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
            CameraNode->setPosition(irr::core::vector3df(0.f, 10.f, 11.f));
        }
        else if(a_TeamIdentifier == ETeamIdentifier::TeamRed)
        {
            CameraNode->setPosition(irr::core::vector3df(0.f, 10.f, -85.f));
        }
	    PlayerNode->setParent(this);
		setParent(CameraNode);

        createAudioEmitter();
        startWalking();

        m_Weapon.setParent(PlayerNode->getJointNode(WeaponJointIndex));
        m_Weapon.disableCollider();
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
    }

    void Player::render()
    {

    }

    const irr::core::aabbox3d<irr::f32>& Player::getBoundingBox() const
    {
        return m_Mesh->getBoundingBox();
    }

    void Player::setLevelCollider(irr::scene::ISceneManager* a_SceneManager,
        irr::scene::ITriangleSelector* a_Level)
    {
        CameraNode->addAnimator(a_SceneManager->createCollisionResponseAnimator(a_Level, PlayerNode, {0.1f, 0.2f, 0.1f}, { 0, -1, 0 }, {0, 1.5f, 0}, 1));
        
        irr::scene::ITriangleSelector* selector = nullptr;
        selector = a_SceneManager->createTriangleSelector(PlayerNode);
        CameraNode->setTriangleSelector(selector);
        selector->drop();
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
        irr::scene::ISceneNodeAnimatorCollisionResponse * animator = 0;

        irr::core::list<irr::scene::ISceneNodeAnimator*>::ConstIterator iterator = CameraNode->getAnimators().begin();

        for(; iterator != CameraNode->getAnimators().end(); ++iterator)
        {
            animator = (irr::scene::ISceneNodeAnimatorCollisionResponse*)(*iterator);
            if(animator->getType() == irr::scene::ESNAT_COLLISION_RESPONSE)
                break;
        }

        if(*TeamIdentifier == ETeamIdentifier::TeamBlue)
        {
             CameraNode->setPosition(irr::core::vector3df(0.f, 10.f, 11.f));
             animator->setTargetNode(CameraNode);
        }
        else if(*TeamIdentifier == ETeamIdentifier::TeamRed)
        {
            CameraNode->setPosition(irr::core::vector3df(0.f, 10.f, -85.f));
            animator->setTargetNode(CameraNode);
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
