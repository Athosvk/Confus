#include <IrrAssimp/IrrAssimp.h>
#include <iostream>
#include "Audio\PlayerAudioEmitter.h"
#include "Player.h"
#include <RakNet/BitStream.h>
#include "EventManager.h"
#include "Flag.h"

namespace ConfusServer
{
    const irr::u32 Player::WeaponJointIndex = 14u;
    const unsigned Player::LightAttackDamage = 10u;
    const unsigned Player::HeavyAttackDamage = 30u;
	Player::Player(irr::IrrlichtDevice* a_Device, long long a_id, ETeamIdentifier a_TeamIdentifier, bool a_MainPlayer)
		: m_Weapon(a_Device->getSceneManager(), irr::core::vector3df(1.0f, 1.0f, 4.0f)),
		irr::scene::ISceneNode(nullptr, a_Device->getSceneManager(), -1),
		TeamIdentifier(new ETeamIdentifier(a_TeamIdentifier)),
		CarryingFlag(new EFlagEnum(EFlagEnum::None))
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

		if (a_MainPlayer) 
        {
			CameraNode = sceneManager->addCameraSceneNodeFPS(0, 100.0f, 0.01f, 1, m_KeyMap, 5, true, 0.15f, false);
			CameraNode->setPosition(irr::core::vector3df(2.5f, 5.f, -30.0f));
			PlayerNode->setParent(this);
			setParent(CameraNode);
		}

        createAudioEmitter();
        startWalking();

        m_Weapon.setParent(PlayerNode->getJointNode(WeaponJointIndex));
        m_Weapon.disableCollider();
    }

	Player::~Player() 
    {
		delete(CarryingFlag);
		delete(TeamIdentifier);
    }

    const irr::core::aabbox3d<irr::f32>& Player::getBoundingBox() const
    {
        return m_Mesh->getBoundingBox();
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

    void Player::setLevelCollider(irr::scene::ISceneManager* a_SceneManager,
        irr::scene::ITriangleSelector* a_Level)
    {
        if(CameraNode != nullptr) 
        {
            CameraNode->addAnimator(a_SceneManager->createCollisionResponseAnimator(a_Level, PlayerNode, {1, 1, 1}, { 0, -1, 0 }));
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
        PlayerNode->setLoopMode(false);
        PlayerNode->setAnimationEndCallback(this);
        PlayerNode->setAnimationSpeed(10);
        m_Attacking = true;
        m_Weapon.enableCollider();
        m_Weapon.resetCollider();
        std::cout << "Attacking on server" << std::endl;
    }

    void Player::startLightAttack()
    {
        if(!m_Attacking) 
        {
            PlayerNode->setFrameLoop(38, 41);
            PlayerNode->setCurrentFrame(38);
            m_Weapon.Damage = LightAttackDamage;
            initializeAttack();
        }
    }

    void Player::startHeavyAttack()
    {
        if(!m_Attacking)
        {
            PlayerNode->setFrameLoop(60, 66);
            PlayerNode->setCurrentFrame(60);
            m_Weapon.Damage = HeavyAttackDamage;
            initializeAttack();
        }
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

    void Player::fixedUpdate()
    {
        updateClient();
    }

    void Player::createAudioEmitter()
    {
        m_FootstepSoundEmitter = new Audio::PlayerAudioEmitter(PlayerNode);
    }

    void Player::updateClient()
    {
        RakNet::BitStream bitstreamOut;
        bitstreamOut.Write(static_cast<RakNet::MessageID>(Networking::Connection::EPacketType::Player));
        bitstreamOut.Write(CameraNode->getAbsolutePosition());
        
        m_Connection->sendMessage(bitstreamOut, PacketReliability::UNRELIABLE);
    }
 

    void Player::setConnection(Networking::Connection* a_Connection)
    {
        m_Connection = a_Connection;

        m_Connection->addFunctionToMap(static_cast<unsigned char>(Networking::Connection::EPacketType::Player), [this](RakNet::BitStream* a_Data)
        {
            unsigned int playerID;
            irr::core::vector3df position;
            irr::core::vector3df rotation;
            EPlayerState state;
            RakNet::Time stateChangeTime;
            int8_t playerHealth;
            bool forwardKeyPressed;
            bool backwardKeyPressed;
            bool leftKeyPressed;
            bool rightKeyPressed;

            a_Data->IgnoreBytes(sizeof(unsigned char));
            a_Data->Read(playerID);
            a_Data->Read(position);
            a_Data->Read(rotation);
            a_Data->Read(state);
            a_Data->Read(stateChangeTime);
            a_Data->Read(playerHealth);
            a_Data->Read(forwardKeyPressed);
            a_Data->Read(backwardKeyPressed);
            a_Data->Read(leftKeyPressed);
            a_Data->Read(rightKeyPressed);

            setRotation(rotation);
             
            if(state == EPlayerState::LightAttacking)
            {
                startLightAttack();
            }
            else if(state == EPlayerState::HeavyAttacking)
            {
                startHeavyAttack();
            }

            // Change this into something better when we are not using irrlichts FPS camera. 
            if(forwardKeyPressed)
            {
                CameraNode->setPosition(irr::core::vector3df(getPosition().X, getPosition().Y, getPosition().Z) + 0.1f);
            } 
            else if(backwardKeyPressed)
            {
                CameraNode->setPosition(irr::core::vector3df(getPosition().X, getPosition().Y, getPosition().Z) - 0.1f);
            }
            else if(leftKeyPressed)
            {
                CameraNode->setPosition(irr::core::vector3df(getPosition().X + 0.1f, getPosition().Y, getPosition().Z));
            }
            else if(rightKeyPressed)
            {
                CameraNode->setPosition(irr::core::vector3df(getPosition().X - 0.1f, getPosition().Y, getPosition().Z));
            }
    });
    }
}
