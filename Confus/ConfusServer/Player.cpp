#include <RakNet/BitStream.h>
#include <iostream>

#include "../ConfusShared/EventManager.h"
#include "../ConfusShared/Flag.h"
#include "../Confusshared/Physics/PhysicsWorld.h"
#include "../Confusshared/Physics/BoxCollider.h"
#include "../Confusshared/Physics/RigidBody.h"
#include "Audio/PlayerAudioEmitter.h"

#include "Player.h"

namespace ConfusServer
{
    const irr::u32 Player::WeaponJointIndex = 14u;
    const unsigned Player::LightAttackDamage = 10u;
    const unsigned Player::HeavyAttackDamage = 30u;

	Player::Player(irr::IrrlichtDevice* a_Device, long long a_id, ConfusShared::ETeamIdentifier a_TeamIdentifier, 
		bool a_MainPlayer,  RakNet::SystemAddress a_SystemAddress, ConfusShared::Physics::PhysicsWorld& a_PhysicsWorld)
		: irr::scene::ISceneNode(nullptr, a_Device->getSceneManager(), -1),
		m_Weapon(a_Device->getSceneManager(), a_PhysicsWorld, irr::core::vector3df(1.0f, 1.0f, 4.0f)),
		TeamIdentifier(a_TeamIdentifier),
		CarryingFlag(ConfusShared::EFlagEnum::None),
        SystemAddress(a_SystemAddress)
    {
        auto sceneManager = a_Device->getSceneManager();
        auto videoDriver = a_Device->getVideoDriver();

        m_Mesh = sceneManager->getMesh("Media/ninja.b3d");
        PlayerNode = sceneManager->addAnimatedMeshSceneNode(m_Mesh, 0, 1);
        PlayerNode->setMaterialFlag(irr::video::E_MATERIAL_FLAG::EMF_LIGHTING, false);

        ID = a_id;

        if(a_TeamIdentifier == ConfusShared::ETeamIdentifier::TeamBlue) 
		{
			PlayerNode->setMaterialTexture(0, videoDriver->getTexture("Media/nskinbl.jpg"));
        }
        else if(a_TeamIdentifier == ConfusShared::ETeamIdentifier::TeamRed) 
		{
            PlayerNode->setMaterialTexture(0, videoDriver->getTexture("Media/nskinrd.jpg"));
        }

        PlayerNode->setPosition(irr::core::vector3df(0, -7.0f, -1.5f));
        PlayerNode->setName({"Player"});

        if(a_TeamIdentifier == ConfusShared::ETeamIdentifier::TeamBlue) {
            PlayerNode->setMaterialTexture(0, videoDriver->getTexture("Media/nskinbl.jpg"));
        }
        else if(a_TeamIdentifier == ConfusShared::ETeamIdentifier::TeamRed) {
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
            CameraNode = sceneManager->addCameraSceneNodeFPS(0, 100.0f, 0.01f, 1, nullptr, 5, true, 0.5f, false, true);
            CameraNode->setFOV(70.f);
            CameraNode->setNearValue(0.1f);
        }
        else
        {
            CameraNode = sceneManager->addCameraSceneNodeFPS(0, 100.0f, 0.01f, 1, m_KeyMap, 5, true, 0.5f, false, false);
        }
        if(a_TeamIdentifier == ConfusShared::ETeamIdentifier::TeamBlue)
        {
            CameraNode->setPosition(irr::core::vector3df(0.f, 10.f, 11.f));
        }
        else if(a_TeamIdentifier == ConfusShared::ETeamIdentifier::TeamRed)
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

    void Player::resetPlayer() 
    {
        if(TeamIdentifier == ConfusShared::ETeamIdentifier::TeamBlue)
        {
            CameraNode->setPosition(irr::core::vector3df(0.f, 10.f, 0.f));
        }
        else if(TeamIdentifier == ConfusShared::ETeamIdentifier::TeamRed)
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
        bitstreamOut.Write(static_cast<RakNet::MessageID>(Networking::EPacketType::Player));
        bitstreamOut.Write(CameraNode->getAbsolutePosition());
        
        m_Connection->sendPacket(bitstreamOut, SystemAddress);
    }
 
    void Player::setConnection(Networking::Connection* a_Connection)
    {
        m_Connection = a_Connection;

        m_Connection->addFunctionToMap(static_cast<unsigned char>(Networking::EPacketType::Player), [this](RakNet::Packet* a_Data)
        {
            RakNet::BitStream bitstreamIn(a_Data->data, a_Data->length, false);

            RakNet::RakNetGUID playerID;
            bitstreamIn.IgnoreBytes(sizeof(unsigned char));
            bitstreamIn.Read(playerID);
            
            // Only continue if the playerID in the packet matches ours.
            if(playerID == static_cast<RakNet::RakNetGUID>(ID))
            {
                irr::core::vector3df rotation;
                EPlayerState state;
                bool forwardKeyPressed;
                bool backwardKeyPressed;
                bool leftKeyPressed;
                bool rightKeyPressed;

                bitstreamIn.Read(rotation);
                bitstreamIn.Read(state);
                bitstreamIn.Read(forwardKeyPressed);
                bitstreamIn.Read(backwardKeyPressed);
                bitstreamIn.Read(leftKeyPressed);
                bitstreamIn.Read(rightKeyPressed);
                
                setRotation(rotation);

                if(state == EPlayerState::LightAttacking)
                {
                    startLightAttack();
                }
                else if(state == EPlayerState::HeavyAttacking)
                {
                    startHeavyAttack();
                }

                auto movementDirection = irr::core::vector3df();
                if(forwardKeyPressed)
                {
                    movementDirection.Z = 1.0f;
                }
                else if(backwardKeyPressed)
                {
                    movementDirection.Z = -1.0f;
                }
                if(leftKeyPressed)
                {
                    movementDirection.X = -1.0f;
                }
                else if(rightKeyPressed)
                {
                    movementDirection.X = 1.0f;
                }
            }
		});
    }
}
