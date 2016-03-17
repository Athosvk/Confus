#include <IrrAssimp/IrrAssimp.h>

#include "Player.h"
#include "EventManager.h"

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

        CameraNode = sceneManager->addCameraSceneNodeFPS(0, 100.0f, 0.5f, -1, m_KeyMap, 5, true, 100.0f, false);
        CameraNode->setPosition(irr::core::vector3df(0, 7.0f, 0));

        PlayerNode->setParent(CameraNode);

        auto collisionBox = sceneManager->addCubeSceneNode(1.0f, PlayerNode, -1, irr::core::vector3d<float>(0.0f, 5.0f, 0.0f), irr::core::vector3d<float>(0.0f, 0.0f, 0.0f), irr::core::vector3d<float>(2.5f, 10.0f, 2.0f));
        collisionBox->setVisible(false);

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
        m_FootstepSoundEmitter->playAttackSound();
        initializeAttack();

    }

    void Player::startHeavyAttack()
    {
        PlayerNode->setFrameLoop(60, 68);
        PlayerNode->setCurrentFrame(60);
        m_Weapon.Damage = HeavyAttackDamage;
        m_FootstepSoundEmitter->playAttackSound();
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