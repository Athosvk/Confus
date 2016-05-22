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
#include "WinScreen.h"
#include "ScoreGUI.h"
#include "HealthGUI.h"
#define DEBUG_CONSOLE
#include "../ConfusShared/Debug.h"
#include "../ConfusShared/TeamIdentifier.h"
#include "../ConfusShared/Physics/BoxCollider.h"

namespace Confus
{
	const int Game::MaxScore = 3; // dont change this for now. Breaks redside score.

    Game::Game(irr::IrrlichtDevice* a_Device, EventManager* a_EventManager) : BaseGame(a_Device, a_EventManager),
        m_PhysicsWorld(m_Device),
        m_MazeGenerator(m_Device, 41, 40, (19 + 20 + 21 + 22 + 23 + 24),  // magic number is just so everytime the first maze is generated it looks the same, not a specific number is chosen
            irr::core::vector2df(19., 20.), m_PhysicsWorld, &m_AudioManager),
        m_PlayerNode(m_Device, m_PhysicsWorld, 1, ETeamIdentifier::TeamBlue, true, &m_AudioManager),
        m_BlueFlag(m_Device, ETeamIdentifier::TeamBlue),
        m_RedFlag(m_Device, ETeamIdentifier::TeamRed),
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

		m_GUI.addElement<HealthGUI>(m_Device, &m_PlayerNode, irr::core::dimension2du(40, 40),
			videoDriver->getTexture("Media/Textures/Heart.png"),
			videoDriver->getTexture("Media/Textures/Blood.png"),
			irr::core::vector2df(0.0f, 0.02f));
    }

    Game::~Game()
    {
        for(size_t i = 0u; i < m_PlayerArray.size(); i++)
        {
            //m_PlayerArray[i]->remove();
            //delete(m_PlayerArray[i]);
        }
        m_Device->getSceneManager()->clear();
    }

    void Game::start()
    {
        m_Device->setWindowCaption(L"Game");

        initializeConnection();
        m_PlayerNode.setConnection(m_Connection.get());

        auto sceneManager = m_Device->getSceneManager();
        m_LevelRootNode = m_Device->getSceneManager()->addEmptySceneNode();
        m_LevelRootNode->setPosition(irr::core::vector3df(1.0f, 1.0f, 1.0f));
        sceneManager->loadScene("Media/IrrlichtScenes/Bases2.irr", nullptr, m_LevelRootNode);
        m_LevelRootNode->setScale(irr::core::vector3df(1.0f, 1.0f, 1.0f));
        m_LevelRootNode->setVisible(true);
        m_Device->setEventReceiver(m_EventManager);
		updateSceneTransformations();
        initializeLevelColliders();

        m_PlayerNode.setEventManager(m_EventManager);
        m_Device->getCursorControl()->setVisible(false);

        m_Connection->addFunctionToMap(static_cast<RakNet::MessageID>(Networking::EPacketType::MainPlayerJoined), [this](RakNet::Packet* a_Data)
        {
            addOwnPlayer(a_Data);
        });

        m_Connection->addFunctionToMap(static_cast<RakNet::MessageID>(Networking::EPacketType::PlayerLeft), [this](RakNet::Packet* a_Data)
        {
            removePlayer(a_Data);
        });

        m_Connection->addFunctionToMap(static_cast<RakNet::MessageID>(Networking::EPacketType::OtherPlayerJoined), [this](RakNet::Packet* a_Data)
        {
            addOtherPlayer(a_Data);
        });

        m_Connection->addFunctionToMap(static_cast<RakNet::MessageID>(Networking::EPacketType::UpdatePosition), [this](RakNet::Packet* a_Data)
        {
            updateOtherPlayer(a_Data);
        });

		m_Connection->addFunctionToMap(static_cast<RakNet::MessageID>(Networking::EPacketType::UpdateHealth), [this](RakNet::Packet* a_Data)
		{
			updateHealth(a_Data);
		});
        
		m_Connection->addFunctionToMap(ID_NO_FREE_INCOMING_CONNECTIONS, [this](RakNet::Packet* a_Data)
		{
			denyConnection(a_Data);
		});
			
		m_Connection->addFunctionToMap(ID_CONNECTION_ATTEMPT_FAILED, [this](RakNet::Packet* a_Data)
		{
			denyConnection(a_Data);
		});

		m_Connection->addFunctionToMap(ID_CONNECTION_LOST, [this](RakNet::Packet* a_Data)
		{
			denyConnection(a_Data);
		});
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
        std::cout << " loaded" << std::endl << "Enter the server's ip address: ";
        std::cin >> serverIP;

        unsigned short serverPort;
        std::cout << "Enter the server's port: ";
        std::cin >> serverPort;

        m_Connection = std::make_unique<Networking::ClientConnection>(serverIP, serverPort);
        m_RedFlag.setConnection(m_Connection.get());
        m_BlueFlag.setConnection(m_Connection.get());
        m_Connection->addFunctionToMap(static_cast<unsigned char>(Networking::EPacketType::MazeChange), [this](RakNet::Packet* a_Data)
        {
            RakNet::BitStream bitstreamIn(a_Data->data, a_Data->length, false);

            int timeMazeChanges, mazeSeed;
            bitstreamIn.IgnoreBytes(sizeof(RakNet::MessageID));
            bitstreamIn.Read(timeMazeChanges);
            bitstreamIn.Read(mazeSeed);
            std::cout << "Update is in " << (timeMazeChanges - static_cast<int>(RakNet::GetTimeMS())) << " ms, the seed is:\t" << mazeSeed << std::endl;
            m_MazeGenerator.refillMainMazeRequest(mazeSeed, timeMazeChanges);
        });

        m_Connection->addFunctionToMap(static_cast<unsigned char>(Networking::EPacketType::ScoreUpdate), [](RakNet::Packet* a_Data)
        {
            RakNet::BitStream bitstreamIn(a_Data->data, a_Data->length, false);

            int redScore, blueScore;
            
            bitstreamIn.IgnoreBytes(sizeof(RakNet::MessageID));
            bitstreamIn.Read(redScore);
            bitstreamIn.Read(blueScore);
            ClientTeamScore::setTeamScore(ETeamIdentifier::TeamRed, redScore);
            ClientTeamScore::setTeamScore(ETeamIdentifier::TeamBlue, blueScore);
            std::cout << "Score updated\tRed score: " << redScore << "\t Blue score: " << blueScore << std::endl;
        });

        m_Connection->addFunctionToMap(static_cast<unsigned char>(Networking::EPacketType::EndOfGame), [this](RakNet::Packet* a_Packet) 
        {
            ETeamIdentifier a_TeamIdentifier;
            RakNet::BitStream inputStream(a_Packet->data, a_Packet->length, false);
            inputStream.IgnoreBytes(sizeof(RakNet::MessageID));
            inputStream.Read(a_TeamIdentifier);

            WinScreen winScreen(m_Device, a_TeamIdentifier, m_EventManager);
            winScreen.run();
            m_Device->setWindowCaption(L"Game");

            if(winScreen.ShouldRestart)
            {
                reset();
            }
            else
            {
                m_ShouldRun = false;
            }
        });
    }

    void Game::handleInput()
    {
        m_PlayerNode.handleInput(*m_EventManager);
        if(m_EventManager->IsKeyDown(irr::KEY_ESCAPE))
        {
            m_ShouldRun = false;
        }
    }

    void Game::update()
    {
        m_Connection->processPackets();
        handleInput();

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

    void Game::reset()
    {
        // We actually would not need to call this method since the server will send score, and new positions.
        m_BlueFlag.returnToStartPosition();
        m_RedFlag.returnToStartPosition();
        m_PlayerNode.respawn();
        ClientTeamScore::setTeamScore(ETeamIdentifier::TeamBlue, 0);
        ClientTeamScore::setTeamScore(ETeamIdentifier::TeamRed, 0);
    }

    void Game::fixedUpdate()
    {
		m_MazeGenerator.fixedUpdate();
		m_PhysicsWorld.stepSimulation(static_cast<float>(FixedUpdateInterval));
        m_PlayerNode.fixedUpdate();
    }

    void Game::end()
    {
        m_BlueFlag.returnToStartPosition();
        m_RedFlag.returnToStartPosition();
    }

    void Game::updateOtherPlayer(RakNet::Packet* a_Data)
    {
        RakNet::BitStream bitstreamIn(a_Data->data, a_Data->length, false);
        
        bitstreamIn.IgnoreBytes(sizeof(RakNet::MessageID));

        long long id;
        bitstreamIn.Read(id);

        for(size_t j = 0u; j < m_PlayerArray.size(); j++)
        {
            for(size_t i = 0u; i < m_PlayerArray.size(); i++)
            {
                if(m_PlayerArray[i]->ID == id)
                {
                    irr::core::vector3df pos;
                    irr::core::vector3df rot;

                    bitstreamIn.Read(pos);
                    bitstreamIn.Read(rot);

                    m_PlayerArray[i]->setPosition(pos);
                    m_PlayerArray[i]->setRotation(rot);
                }
            }
            bitstreamIn.Read(id);
        }
    }

    void Game::updateHealth(RakNet::Packet* a_Data)
    {
        RakNet::BitStream inputStream(a_Data->data, a_Data->length, false);
        inputStream.IgnoreBytes(sizeof(RakNet::MessageID));

        long long id;
        inputStream.Read(id);

		for (size_t i = 0u; i < m_PlayerArray.size(); i++)
		{
			if (m_PlayerArray[i]->ID == id)
			{
				int health;
				EHitIdentifier hitIdentifier;

				inputStream.Read(health);
				inputStream.Read(hitIdentifier);

				if (health > m_PlayerArray[i]->getHealthInstance()->getHealth())
				{
					m_PlayerArray[i]->getHealthInstance()->heal(health - m_PlayerArray[i]->getHealthInstance()->getHealth());
				}
				else if (health < m_PlayerArray[i]->getHealthInstance()->getHealth())
				{
					m_PlayerArray[i]->getHealthInstance()->damage(m_PlayerArray[i]->getHealthInstance()->getHealth() - health, hitIdentifier);
				}
				break;
			}
		}
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
    //need to test of the guid.g is the right one, and not the one from the server
    void Game::addOwnPlayer(RakNet::Packet* a_Data)
    {
        RakNet::BitStream bitstreamIn(a_Data->data, a_Data->length, false);

        bitstreamIn.IgnoreBytes(sizeof(RakNet::MessageID));

        bitstreamIn.Read(m_PlayerNode.ID);
        bitstreamIn.Read(m_PlayerNode.TeamIdentifier);
        m_PlayerNode.respawn();
        m_PlayerNode.updateColor(m_Device);

        size_t size;
        bitstreamIn.Read(size);

        for(size_t i = 0u; i < size; i++)
        {
            long long id;
            ETeamIdentifier teamID;

            bitstreamIn.Read(id);
            bitstreamIn.Read(teamID);

            Player* newPlayer = new Player(m_Device, m_PhysicsWorld, id, teamID, false, &m_AudioManager);
            m_PlayerArray.push_back(newPlayer);
        }

        m_PlayerArray.push_back(&m_PlayerNode);
    }

    void Game::addOtherPlayer(RakNet::Packet* a_Data)
    {
        RakNet::BitStream bitstreamIn(a_Data->data, a_Data->length, false);

        bitstreamIn.IgnoreBytes(sizeof(RakNet::MessageID));

        long long id;
        ETeamIdentifier teamID;

        bitstreamIn.Read(id);
        bitstreamIn.Read(teamID);

        Player* newPlayer = new Player(m_Device, m_PhysicsWorld, id, teamID, false, &m_AudioManager);
        m_PlayerArray.push_back(newPlayer);
    }

    void Game::removePlayer(RakNet::Packet* a_Data)
    {
        RakNet::BitStream bitstreamIn(a_Data->data, a_Data->length, false);

        bitstreamIn.IgnoreBytes(sizeof(RakNet::MessageID));

        long long id;

        bitstreamIn.Read(id);

        for(size_t i = 0u; i < m_PlayerArray.size(); i++)
        {
            if(m_PlayerArray[i]->ID == id)
            {
                m_PlayerArray[i]->remove();
                delete(m_PlayerArray[i]);
                m_PlayerArray.erase(m_PlayerArray.begin() + i);
                break;
            }
        }
        m_PlayerNode.fixedUpdate();
    }

	void Game::denyConnection(RakNet::Packet* a_Data)
	{
		RakNet::BitStream inputStream(a_Data->data, a_Data->length, false);
		RakNet::MessageID messageID;

		inputStream.Read(messageID);

		switch (messageID)
		{
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				std::cout << "Server is full" << std::endl;
			default:
				std::cout << "Could not connect to the server" << std::endl;
		}

		m_ShouldRun = false;
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
