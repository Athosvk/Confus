#include <IrrAssimp/IrrAssimp.h>
#include <RakNet/BitStream.h>

#include "Player.h"
#include "EventManager.h"
#include "Flag.h"
#include "Audio/PlayerAudioEmitter.h"
#include "../Confusshared/Physics/PhysicsWorld.h"
#include "../Confusshared/Physics/BoxCollider.h"
#include "../Confusshared/Physics/RigidBody.h"
#include "../ConfusShared/Networking/BitStreamStruct.h"

namespace ConfusServer
{
    const irr::u32 Player::WeaponJointIndex = 14u;
    const unsigned Player::LightAttackDamage = 10u;
    const unsigned Player::HeavyAttackDamage = 30u;
    const int Player::UserTimedOutTime = 20;

    Player::Player(irr::IrrlichtDevice* a_Device, char a_id, ETeamIdentifier a_TeamIdentifier, bool a_MainPlayer)
        : m_Weapon(a_Device->getSceneManager(), irr::core::vector3df(1.0f, 1.0f, 4.0f)),
        irr::scene::ISceneNode(nullptr, a_Device->getSceneManager(), -1),
        TeamIdentifier(a_TeamIdentifier),
        CarryingFlag(EFlagEnum::None)
    {
        auto sceneManager = a_Device->getSceneManager();
        auto videoDriver = a_Device->getVideoDriver();

        IrrAssimp irrAssimp(sceneManager);
        m_Mesh = sceneManager->getMesh("Media/ninja.b3d");
        PlayerNode = sceneManager->addAnimatedMeshSceneNode(m_Mesh, 0, 1);
        PlayerNode->setMaterialFlag(irr::video::E_MATERIAL_FLAG::EMF_LIGHTING, false);

        ID = a_id;

        if(a_TeamIdentifier == ETeamIdentifier::TeamBlue)
        {
            PlayerNode->setMaterialTexture(0, videoDriver->getTexture("Media/nskinbl.jpg"));
        }
        else if(a_TeamIdentifier == ETeamIdentifier::TeamRed)
        {
            PlayerNode->setMaterialTexture(0, videoDriver->getTexture("Media/nskinrd.jpg"));
        }

        PlayerNode->setPosition(irr::core::vector3df(0, -7.0f, -1.5f));
        PlayerNode->setName({ "Player" });

        if(a_MainPlayer)
        {
            CameraNode = sceneManager->addCameraSceneNodeFPS(0, 100.0f, 0.01f, 1, nullptr, 5, true, 0.5f, false, true);
            CameraNode->setFOV(70.f);
            CameraNode->setNearValue(0.1f);
        }
        else
        {
            CameraNode = sceneManager->addCameraSceneNodeFPS(0, 100.0f, 0.01f, 1, m_KeyMap, 5, true, 0.5f, false, false);
        }

        PlayerNode->setParent(this);
        setParent(CameraNode);
        resetPlayer();

        createAudioEmitter();
        startWalking();

        m_Weapon.setParent(PlayerNode->getJointNode(WeaponJointIndex));
        m_Weapon.disableCollider();
        m_LastUpdateTime = RakNet::GetTimeMS() / 1000;
    }

    void Player::resetPlayer()
    {
        if(TeamIdentifier == ETeamIdentifier::TeamBlue)
        {
            CameraNode->setPosition(irr::core::vector3df(0.f, 10.f, 11.f));
        }
        else if(TeamIdentifier == ETeamIdentifier::TeamRed)
        {
            CameraNode->setPosition(irr::core::vector3df(0.f, 10.f, -85.f));
        }
    }


    Player::~Player()
    {
    }

    const irr::core::aabbox3d<irr::f32>& Player::getBoundingBox() const
    {
        return m_Mesh->getBoundingBox();
    }

    void Player::render()
    {

    }

    void Player::setLevelCollider(irr::scene::ISceneManager* a_SceneManager, irr::scene::ITriangleSelector* a_Level)
    {
        if(CameraNode != nullptr)
        {
            CameraNode->addAnimator(a_SceneManager->createCollisionResponseAnimator(a_Level, PlayerNode, { 1, 1, 1 }, { 0, -1, 0 }));
        }
        else
        {
            PlayerNode->setTriangleSelector(a_Level);
            PlayerNode->addAnimator(a_SceneManager->createCollisionResponseAnimator(a_Level,
                PlayerNode, PlayerNode->getBoundingBox().getExtent() / 10, irr::core::vector3df(0.0f, 0.0f, 0.0f)));
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
        m_Attacking = true;
        PlayerNode->setLoopMode(false);
        PlayerNode->setAnimationEndCallback(this);
        PlayerNode->setAnimationSpeed(10);
        m_Weapon.enableCollider();
        m_Weapon.resetCollider();
    }

    void Player::startLightAttack()
    {
        if(!m_Attacking)
        {
            initializeAttack();
            PlayerNode->setFrameLoop(38, 41);
            PlayerNode->setCurrentFrame(38);

            m_Weapon.Damage = LightAttackDamage;
            PlayerState = EPlayerState::LightAttacking;
        }
    }

    void Player::startHeavyAttack()
    {
        if(!m_Attacking)
        {
            initializeAttack();
            PlayerNode->setFrameLoop(60, 66);
            PlayerNode->setCurrentFrame(60);
            m_Weapon.Damage = HeavyAttackDamage;
            PlayerState = EPlayerState::HeavyAttacking;
        }
    }

    void Player::OnAnimationEnd(irr::scene::IAnimatedMeshSceneNode* node)
    {
        if(m_Attacking)
        {
            m_Attacking = false;
            m_Weapon.disableCollider();
            startWalking();
            PlayerState = EPlayerState::Idle;
        }
    }

    void Player::update()
    {
    }

    void Player::fixedUpdate()
    {
    }

    void Player::createAudioEmitter()
    {
    }

    bool Player::userTimedOut()
    {
        int currentTime = static_cast<int>((RakNet::GetTimeMS()) / 1000);
        return currentTime > m_LastUpdateTime + UserTimedOutTime;
    }

    void Player::updateFromClient(RakNet::BitStream* a_Data)
    {
        ConfusShared::Networking::PlayerInfo playerInfo;
        a_Data->IgnoreBytes(sizeof(unsigned char));
        a_Data->Read(playerInfo);


        m_LastUpdateTime = RakNet::GetTimeMS() / 1000;

        setRotation(playerInfo.rotation);

        if(PlayerState == EPlayerState::Idle)
        {
            if(playerInfo.leftMouseButtonPressed)
            {
                startLightAttack();
            }
            else if(playerInfo.rightMouseButtonPressed)
            {
                startHeavyAttack();
            }
        }

        // TODO: Use bullet instead of setting position manually
        if(playerInfo.forwardKeyPressed)
        {
            CameraNode->setPosition(irr::core::vector3df(CameraNode->getPosition().X, CameraNode->getPosition().Y, CameraNode->getPosition().Z + 0.1f));
        }
        else if(playerInfo.backwardKeyPressed)
        {
            CameraNode->setPosition(irr::core::vector3df(CameraNode->getPosition().X, CameraNode->getPosition().Y, CameraNode->getPosition().Z) - 0.1f);
        }
        else if(playerInfo.leftKeyPressed)
        {
            CameraNode->setPosition(irr::core::vector3df(CameraNode->getPosition().X - 0.1f, CameraNode->getPosition().Y, CameraNode->getPosition().Z));
        }
        else if(playerInfo.rightKeyPressed)
        {
            CameraNode->setPosition(irr::core::vector3df(CameraNode->getPosition().X + 0.1f, CameraNode->getPosition().Y, CameraNode->getPosition().Z));
        }
    }
}
