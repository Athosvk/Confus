#include <Irrlicht/irrlicht.h>
#include <time.h>
#include <iostream>
#include <RakNet/BitStream.h>
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/GetTime.h>

#include "Game.h"
#include "Player.h"
#include "Flag.h"
#include "FlagGUI.h"

#include "ScoreGUI.h"
#define DEBUG_CONSOLE
#include "../ConfusShared/Debug.h"
#include "../ConfusShared/TeamIdentifier.h"
#include "../ConfusShared/Physics/BoxCollider.h"
#include "../ConfusShared/Networking/BitStreamStruct.h"

namespace Confus
{
    const double Game::FixedUpdateInterval = 0.03;
    const double Game::MaxFixedUpdateInterval = 0.1;
	const int Game::MaxScore = 3; // dont change this for now. Breaks redside score.

    Game::Game(irr::core::dimension2d<irr::u32> a_Resolution)
        : m_Device(irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_OPENGL, a_Resolution)),
		m_PhysicsWorld(m_Device),
		m_MazeGenerator(m_Device, 41, 40,(19+20+21+22+23+24),  // magic number is just so everytime the first maze is generated it looks the same, not a specific number is chosen
			irr::core::vector2df(19., 20.), m_PhysicsWorld, &m_AudioManager),
        m_PlayerNode(m_Device, m_PhysicsWorld, 1, ETeamIdentifier::TeamBlue, true, &m_AudioManager),
        m_BlueFlag(m_Device, ETeamIdentifier::TeamBlue, m_PhysicsWorld),
        m_RedFlag(m_Device, ETeamIdentifier::TeamRed, m_PhysicsWorld),
        m_RedRespawnFloor(m_Device, m_PhysicsWorld, irr::core::vector3df(0.f, 3.45f, 11.f)),
        m_BlueRespawnFloor(m_Device, m_PhysicsWorld, irr::core::vector3df(0.f, 3.45f, -83.f)),
        m_GUI(m_Device, &m_PlayerNode, &m_AudioManager),
		m_Announcer(&m_RedFlag,&m_BlueFlag,&m_PlayerNode, &m_AudioManager)
    {
		auto videoDriver = m_Device->getVideoDriver();
		m_GUI.addElement<FlagGUI>(m_Device, &m_BlueFlag, irr::core::dimension2du(50, 50),
					videoDriver->getTexture("Media/Textures/FlagUIImage.png"),
					videoDriver->getTexture("Media/Textures/ExclamationMark.png"),
					irr::core::vector2df(0.44f, 0.0f), false);

		m_GUI.addElement<FlagGUI>(m_Device, &m_RedFlag, irr::core::dimension2du(50, 50),
			videoDriver->getTexture("Media/Textures/MirroredFlagUIImage.png"),
			videoDriver->getTexture("Media/Textures/MirroredExclamationMark.png"),
			irr::core::vector2df(0.56f, 0.0f), true);

		m_GUI.addElement<ScoreGUI>(m_Device, &m_RedFlag, irr::core::dimension2du(30, 30),
			videoDriver->getTexture("Media/Textures/Orb.png"), irr::core::vector2df(0.59f, 0.061f));

		m_GUI.addElement<ScoreGUI>(m_Device, &m_BlueFlag, irr::core::dimension2du(30, 30),
			videoDriver->getTexture("Media/Textures/Orb.png"), irr::core::vector2df(0.45f, 0.061f));
    }

    Game::~Game()
    {
        for(size_t i = 0u; i < m_PlayerArray.size(); i++)
        {
            //m_PlayerArray[i]->remove();
            //delete(m_PlayerArray[i]);
        }
    }

    void Game::run()
    {
        initializeConnection();

        auto sceneManager = m_Device->getSceneManager();
        m_LevelRootNode = m_Device->getSceneManager()->addEmptySceneNode();

        m_LevelRootNode->setPosition(irr::core::vector3df(1.0f, 1.0f, 1.0f));
        sceneManager->loadScene("Media/IrrlichtScenes/Bases2.irr", nullptr, m_LevelRootNode);
        m_LevelRootNode->setScale(irr::core::vector3df(1.0f, 1.0f, 1.0f));
        m_LevelRootNode->setVisible(true);
        
        m_Device->setEventReceiver(&m_EventManager);

		updateSceneTransformations();
        initializeLevelColliders();

        m_PlayerNode.setEventManager(&m_EventManager);
        m_Device->getCursorControl()->setVisible(false);

        m_Connection->addFunctionToMap(static_cast<RakNet::MessageID>(Networking::EPacketType::MainPlayerJoined), [this](RakNet::BitStream* a_Data)
        {
            addMainPlayer(a_Data);
        });

        m_Connection->addFunctionToMap(static_cast<RakNet::MessageID>(Networking::EPacketType::OtherPlayerJoined), [this](RakNet::BitStream* a_Data)
        {
            addOtherPlayer(a_Data);
        });

        m_Connection->addFunctionToMap(static_cast<RakNet::MessageID>(Networking::EPacketType::PlayerLeft), [this](RakNet::BitStream* a_Data)
        {
            removePlayer(a_Data);
        });

        m_Connection->addFunctionToMap(static_cast<RakNet::MessageID>(Networking::EPacketType::UpdatePosition), [this](RakNet::BitStream* a_Data)
        {
            a_Data->IgnoreBytes(sizeof(RakNet::MessageID));

            ConfusShared::Networking::PlayerInfo playerInfo;
            a_Data->Read(playerInfo);

			for (size_t i = 0u; i < m_PlayerArray.size(); i++)
			{
                m_PlayerArray[i]->updateFromServer(playerInfo);
			}
        });
      
        while(m_Device->run())
        {
            m_Connection->processPackets();
			handleInput();
            update();
            processFixedUpdates();
            render();
        }
    }

	void Game::initializeLevelColliders()
	{
		irr::core::array<irr::scene::ISceneNode*> nodes;
		m_Device->getSceneManager()->getSceneNodesFromType(irr::scene::ESNT_ANY, nodes, m_LevelRootNode);
		for(irr::u32 i = 0; i < nodes.size(); ++i)
		{
			irr::scene::ISceneNode* node = nodes[i];
			Physics::Collider* collider = nullptr;

			if(node->isVisible())
			{
				switch(node->getType())
				{
				case irr::scene::ESNT_CUBE:
				case irr::scene::ESNT_ANIMATED_MESH:
				case irr::scene::ESNT_MESH:
					if(std::string(node->getName()).find("Ground", 0) != std::string::npos)
					{
						collider = m_PhysicsWorld.createBoxCollider(node, Physics::ECollisionFilter::LevelStatic,
							Physics::ECollisionFilter::Player | Physics::ECollisionFilter::Interactable);
					}
					else if (std::string(node->getName()).find("Basefolder", 0) == std::string::npos)
					{
						collider = m_PhysicsWorld.createBoxCollider(node->getScale(), node, Physics::ECollisionFilter::LevelStatic | Physics::ECollisionFilter::Interactable,
							Physics::ECollisionFilter::Player);
                    }
                    else
                    {
                        break;
                    }
					collider->getRigidBody()->makeStatic();
					break;
				case irr::scene::ESNT_SPHERE:
				case irr::scene::ESNT_TERRAIN:
				case irr::scene::ESNT_OCTREE:
					break;
				default:
					break;
				}
			}
		}
	}

    void Game::initializeConnection()
    {
        std::string serverIP;
        std::cout << "Enter the server's ip address: ";
        std::cin >> serverIP;

        unsigned short serverPort;
        std::cout << "Enter the server's port: ";
        std::cin >> serverPort;

        m_Connection = std::make_unique<Networking::ClientConnection>(serverIP, serverPort);
        m_PlayerNode.setConnection(m_Connection.get());

        m_Connection->addFunctionToMap(static_cast<unsigned char>(Networking::EPacketType::MazeChange), [this](RakNet::BitStream* a_InputStream)
		{
            int timeMazeChanges, mazeSeed;
           // RakNet::BitStream inputStream(a_Packet->data, a_Packet->length, false);
            a_InputStream->IgnoreBytes(sizeof(RakNet::MessageID));
            a_InputStream->Read(timeMazeChanges);
            a_InputStream->Read(mazeSeed);
            m_MazeGenerator.refillMainMazeRequest(mazeSeed, timeMazeChanges);
        });

        m_Connection->addFunctionToMap(static_cast<unsigned char>(Networking::EPacketType::ScoreUpdate), [](RakNet::BitStream* a_InputStream)
		{
            int redScore, blueScore;
            
            a_InputStream->IgnoreBytes(sizeof(RakNet::MessageID));
            a_InputStream->Read(redScore);
            a_InputStream->Read(blueScore);
            ClientTeamScore::setTeamScore(ETeamIdentifier::TeamRed, redScore);
            ClientTeamScore::setTeamScore(ETeamIdentifier::TeamBlue, blueScore);
        });
    }

    void Game::handleInput()
    {
        m_PlayerNode.handleInput(m_EventManager);
    }

    void Game::update()
    {
        m_PreviousTicks = m_CurrentTicks;
        m_CurrentTicks = m_Device->getTimer()->getTime();
        m_DeltaTime = (m_CurrentTicks - m_PreviousTicks) / 1000.0;

        m_PlayerNode.update();
		m_GUI.update();
        m_Listener.setPosition(m_PlayerNode.getAbsolutePosition());

        irr::core::matrix4 playerRotation(m_PlayerNode.getAbsoluteTransformation());
        irr::core::vector3df forwardVector = irr::core::vector3df(playerRotation[8], playerRotation[9], playerRotation[10] );
        irr::core::vector3df upVector = irr::core::vector3df(playerRotation[4], playerRotation[5], playerRotation[6]);
        m_Listener.setDirection(forwardVector, upVector);     

		static float timer = 0.0f;
		timer += static_cast<float>(m_DeltaTime);
		if(timer >= 3.0f)
		{
			m_BlueRespawnFloor.enableCollision();
			m_RedRespawnFloor.enableCollision();
		}
		if(timer >= 20.0f)
		{
			timer = 0.0f;
			m_MazeGenerator.refillMainMaze(static_cast<int>(time(0)));
			m_BlueRespawnFloor.disableCollision();
			m_RedRespawnFloor.disableCollision();
		}
    }

    void Game::processFixedUpdates()
    {
        m_FixedUpdateTimer += m_DeltaTime;
        m_FixedUpdateTimer = irr::core::min_(m_FixedUpdateTimer, MaxFixedUpdateInterval);
        while(m_FixedUpdateTimer >= FixedUpdateInterval)
        {
            m_FixedUpdateTimer -= FixedUpdateInterval;
            fixedUpdate();
        }
    }

    void Game::fixedUpdate()
    {
		m_MazeGenerator.fixedUpdate();
		m_PhysicsWorld.stepSimulation(static_cast<float>(FixedUpdateInterval));
    }

	void Game::updateSceneTransformations()
	{
		std::function<void(irr::scene::ISceneNode* a_Node)> updateDownwards = [&updateDownwards](irr::scene::ISceneNode* a_Node)
		{
			a_Node->updateAbsolutePosition();
			auto children = a_Node->getChildren();
			for(auto child : children)
			{
				updateDownwards(child);
			}
		};
		updateDownwards(m_Device->getSceneManager()->getRootSceneNode());
	}

	void Game::addMainPlayer (RakNet::BitStream* a_Data)
    {
		char newPlayerID;
		ETeamIdentifier newPlayerIdentifier;
		a_Data->IgnoreBytes(sizeof(RakNet::MessageID));
		a_Data->Read(newPlayerID);
		a_Data->Read(newPlayerIdentifier);

        //Set our own data
        m_PlayerNode.ID = m_Connection->ClientID = newPlayerID;
        m_PlayerNode.TeamIdentifier = newPlayerIdentifier;
        m_PlayerNode.respawn();
        m_PlayerNode.updateColor(m_Device);
        m_PlayerArray.push_back(&m_PlayerNode);

        // Add other players
        size_t size;
        a_Data->Read(size);

        for(size_t i = 0u; i < size; i++)
        {
            char id;
            ETeamIdentifier teamID;

            a_Data->Read(id);
            a_Data->Read(teamID);

            if(id != m_PlayerNode.ID)
            {
                Player* newPlayer = new Player(m_Device, m_PhysicsWorld, id, teamID, false, &m_AudioManager);
                m_PlayerArray.push_back(newPlayer);
            }
        }
    }

    void Game::addOtherPlayer(RakNet::BitStream* a_Data)
    {
        char newPlayerID;
        a_Data->IgnoreBytes(sizeof(RakNet::MessageID));
        a_Data->Read(newPlayerID);

        if(newPlayerID != m_PlayerNode.ID)
        {
            ETeamIdentifier newPlayerIdentifier;
            a_Data->Read(newPlayerIdentifier);
            Player* newPlayer = new Player(m_Device, m_PhysicsWorld, newPlayerID, newPlayerIdentifier, false, &m_AudioManager);
            m_PlayerArray.push_back(newPlayer);
        }
    }

    void Game::removePlayer(RakNet::BitStream* a_Data)
    {
		a_Data->IgnoreBytes(sizeof(RakNet::MessageID));

        char id;

		a_Data->Read(id);

        for(size_t i = 0u; i < m_PlayerArray.size(); i++)
        {
			if (m_PlayerNode.ID == id)
			{
				//m_ShouldRun = false; //TODO: Uncomment when merged with  develop
			}
            else if(m_PlayerArray[i]->ID == id)
            {
                m_PlayerArray[i]->remove();
                delete(m_PlayerArray[i]);
                m_PlayerArray.erase(m_PlayerArray.begin() + i);
                break;
            }
        }
    }

    void Game::render()
    {
        m_Device->getVideoDriver()->beginScene(true, true, irr::video::SColor(255, 100, 101, 140));
        m_Device->getSceneManager()->drawAll();
        m_Device->getGUIEnvironment()->drawAll();
		m_PhysicsWorld.drawDebugInformation();
        m_Device->getVideoDriver()->endScene();
    }
}
