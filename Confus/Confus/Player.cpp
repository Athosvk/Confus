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
    Player::Player(irr::IrrlichtDevice* a_Device)
        : m_Weapon(a_Device->getSceneManager(), irr::core::vector3df(1.0f, 1.0f, 4.0f))
    {
        auto sceneManager = a_Device->getSceneManager();
        auto videoDriver = a_Device->getVideoDriver();

        IrrAssimp irrAssimp(sceneManager);
        irr::scene::IAnimatedMesh* mesh = sceneManager->getMesh("Media/ninja.b3d");

        PlayerNode = sceneManager->addAnimatedMeshSceneNode(mesh);
        PlayerNode->setMaterialFlag(irr::video::E_MATERIAL_FLAG::EMF_LIGHTING, false);
        PlayerNode->setMaterialTexture(0, videoDriver->getTexture("Media/nskinbl.jpg"));
        PlayerNode->setPosition(irr::core::vector3df(0, -7.0f, -1.5f));

        m_KeyMap[1].Action = irr::EKA_MOVE_FORWARD;
        m_KeyMap[1].KeyCode = irr::KEY_KEY_W;

        m_KeyMap[2].Action = irr::EKA_MOVE_BACKWARD;
        m_KeyMap[2].KeyCode = irr::KEY_KEY_S;

        m_KeyMap[3].Action = irr::EKA_STRAFE_LEFT;
        m_KeyMap[3].KeyCode = irr::KEY_KEY_A;

        m_KeyMap[4].Action = irr::EKA_STRAFE_RIGHT;
        m_KeyMap[4].KeyCode = irr::KEY_KEY_D;

        m_KeyMap[5].Action = irr::EKA_JUMP_UP;
        m_KeyMap[5].KeyCode = irr::KEY_SPACE;

        CameraNode = sceneManager->addCameraSceneNodeFPS(0, 100.0f, 0.01f, -1, m_KeyMap, 5, true, 100.0f, false);
        CameraNode->setPosition(irr::core::vector3df(2.5f, 150.0f, 5.0f));

        PlayerNode->setParent(CameraNode);
        createAudioEmitter();
        startWalking();

        m_Weapon.setParent(PlayerNode->getJointNode(WeaponJointIndex));
        m_Weapon.disableCollider();
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

    void Player::setLevelCollider(irr::scene::ISceneManager* a_SceneManager,
        irr::scene::ITriangleSelector* a_Level)
    {
        auto triangleSelector = a_SceneManager->createTriangleSelectorFromBoundingBox(PlayerNode);
        CameraNode->setTriangleSelector(triangleSelector);
        CameraNode->addAnimator(a_SceneManager->createCollisionResponseAnimator(a_Level,
            CameraNode, PlayerNode->getBoundingBox().getExtent() / 10));
        triangleSelector->drop();
    }

    void Player::setFlagCollider(irr::scene::ISceneManager* a_SceneManager, irr::scene::IMetaTriangleSelector* a_Flags) {
        auto anim = a_SceneManager->createCollisionResponseAnimator(a_Flags, PlayerNode);
        Collider col(anim);
        col.setCallback([this](irr::scene::ISceneNode* a_CollidedNode)
        {
            auto name = a_CollidedNode->getName();
            //TODO Search correct flag and capture that flag
            if(a_CollidedNode->getID() == static_cast<int>(ETeamIdentifier::TEAM_RED))
            {
                //TODO captureFlag(this);
                return true;
            }
            else if(a_CollidedNode->getID() == static_cast<int>(ETeamIdentifier::TEAM_BLUE))
            {
                //TODO captureFlag(this);
                return true;
            }
            return false;
        });
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
        initializeAttack();
    }

    void Player::startHeavyAttack()
    {
        PlayerNode->setFrameLoop(60, 66);
        PlayerNode->setCurrentFrame(60);
        m_Weapon.Damage = HeavyAttackDamage;
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
        m_FootstepSoundEmitter->updatePosition();

        int frameNumber = static_cast<int>(PlayerNode->getFrameNr());
        if(frameNumber == 0 || frameNumber == 6)
        {
            m_FootstepSoundEmitter->playFootStepSound();
        }
    }

    void Player::createAudioEmitter()
    {
        m_FootstepSoundEmitter = new Audio::PlayerAudioEmitter(PlayerNode);
    }
}