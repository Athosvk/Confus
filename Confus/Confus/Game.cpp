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
#define DEBUG_CONSOLE
#include "../ConfusShared/Debug.h"
#include "../ConfusShared/TeamIdentifier.h"
#include "../ConfusShared/Physics/BoxCollider.h"
#include "../ConfusShared/Networking/BitStreamStruct.h"

namespace Confus
{
    const int Game::MaxScore = 3; // dont change this for now. Breaks redside score.

    Game::Game(irr::IrrlichtDevice* a_Device, EventManager* a_EventManager) : BaseGame(a_Device, a_EventManager),
        m_PhysicsWorld(m_Device),
        m_MazeGenerator(m_Device, 41, 40, (19 + 20 + 21 + 22 + 23 + 24),  // magic number is just so everytime the first maze is generated it looks the same, not a specific number is chosen
            irr::core::vector2df(19., 20.), m_PhysicsWorld, &m_AudioManager),
        m_PlayerNode(m_Device, m_PhysicsWorld, 0, ETeamIdentifier::TeamBlue, true, &m_AudioManager),
        m_BlueFlag(m_Device, ETeamIdentifier::TeamBlue, m_PhysicsWorld),
        m_RedFlag(m_Device, ETeamIdentifier::TeamRed, m_PhysicsWorld),
        m_RedRespawnFloor(m_Device, m_PhysicsWorld, irr::core::vector3df(0.f, 3.45f, 11.f)),
        m_BlueRespawnFloor(m_Device, m_PhysicsWorld, irr::core::vector3df(0.f, 3.45f, -83.f)),
        m_GUI(m_Device, &m_PlayerNode, &m_AudioManager),
        m_Announcer(&m_RedFlag, &m_BlueFlag, &m_PlayerNode, &m_AudioManager)
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
        for(size_t i = 0u; i < m_PlayerArray.size(); ++i)
        {
            //m_PlayerArray[i]->remove();
            //delete(m_PlayerArray[i]);
        }
        m_Device->getSceneManager()->clear();
    }

    void Game::start()
    {

        initializeConnection();

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

            for(size_t i = 0u; i < m_PlayerArray.size(); ++i)
            {
				if (m_PlayerArray[i]->ID == playerInfo.playerID)
				{
					m_PlayerArray[i]->updateFromServer(playerInfo);
				}
            }
        });

        m_Connection->addFunctionToMap(static_cast<RakNet::MessageID>(Networking::EPacketType::UpdateHealth), [this](RakNet::BitStream* a_Data)
        {
            updateHealth(a_Data);
        });

        m_Connection->addFunctionToMap(ID_NO_FREE_INCOMING_CONNECTIONS, [this](RakNet::BitStream* a_Data)
        {
            denyConnection(a_Data);
        });

        m_Connection->addFunctionToMap(ID_CONNECTION_ATTEMPT_FAILED, [this](RakNet::BitStream* a_Data)
        {
            denyConnection(a_Data);
        });

        m_Connection->addFunctionToMap(ID_CONNECTION_LOST, [this](RakNet::BitStream* a_Data)
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
                    else if(std::string(node->getName()).find("Basefolder", 0) == std::string::npos)
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
        m_PlayerNode.setConnection(m_Connection.get());

        m_Connection->addFunctionToMap(static_cast<unsigned char>(Networking::EPacketType::MazeChange), [this](RakNet::BitStream* a_Data)
        {
            int timeMazeChanges, mazeSeed;
            a_Data->IgnoreBytes(sizeof(RakNet::MessageID));
            a_Data->Read(timeMazeChanges);
            a_Data->Read(mazeSeed);
            m_MazeGenerator.refillMainMazeRequest(mazeSeed, timeMazeChanges);
        });

        m_Connection->addFunctionToMap(static_cast<unsigned char>(Networking::EPacketType::ScoreUpdate), [](RakNet::BitStream* a_Data)
        {
            int redScore, blueScore;

            a_Data->IgnoreBytes(sizeof(RakNet::MessageID));
            a_Data->Read(redScore);
            a_Data->Read(blueScore);
            ClientTeamScore::setTeamScore(ETeamIdentifier::TeamRed, redScore);
            ClientTeamScore::setTeamScore(ETeamIdentifier::TeamBlue, blueScore);
        });

        m_Connection->addFunctionToMap(static_cast<unsigned char>(Networking::EPacketType::EndOfGame), [this](RakNet::BitStream* a_Data)
        {
            ETeamIdentifier a_TeamIdentifier;
            a_Data->IgnoreBytes(sizeof(RakNet::MessageID));
            a_Data->Read(a_TeamIdentifier);

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
        irr::core::vector3df forwardVector = irr::core::vector3df(playerRotation[8], playerRotation[9], playerRotation[10]);
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


    void Game::updateHealth(RakNet::BitStream* a_Data)
    {
        a_Data->IgnoreBytes(sizeof(RakNet::MessageID));

        long long id;
        a_Data->Read(id);

        for(size_t i = 0u; i < m_PlayerArray.size(); ++i)
        {
            if(m_PlayerArray[i]->ID == id)
            {
                int health;
                EHitIdentifier hitIdentifier;

                a_Data->Read(health);
                a_Data->Read(hitIdentifier);

                if(health > m_PlayerArray[i]->getHealthInstance()->getHealth())
                {
                    m_PlayerArray[i]->getHealthInstance()->heal(health - m_PlayerArray[i]->getHealthInstance()->getHealth());
                }
                else if(health < m_PlayerArray[i]->getHealthInstance()->getHealth())
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

    void Game::addMainPlayer(RakNet::BitStream* a_Data)
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

        for(size_t i = 0u; i < size; ++i)
        {
            char id;
            ETeamIdentifier teamID;

            a_Data->Read(id);
            a_Data->Read(teamID);

            if(id != m_PlayerNode.ID)
            {
                Player* newPlayer = new Player(m_Device, m_PhysicsWorld, id, teamID, false, &m_AudioManager);
				newPlayer->updateColor(m_Device);
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

        for(size_t i = 0u; i < m_PlayerArray.size(); ++i)
        {
			if (id == m_PlayerArray[i]->ID)
			{
				if (m_PlayerNode.ID == id)
				{
					m_ShouldRun = false;
				}
				else if (m_PlayerNode.ID != id)
				{
                    if(m_PlayerArray[i]->FlagPointer != nullptr)
                    {
                        m_PlayerArray[i]->FlagPointer->drop(m_PlayerArray[i]);
                    }
                    // Remove children of the node
					m_PlayerArray[i]->removeAll(); 
                    // Remove node
                    m_PlayerArray[i]->remove();
					delete(m_PlayerArray[i]);
					m_PlayerArray.erase(m_PlayerArray.begin() + i);
				}
			}
        }
    }

    void Game::denyConnection(RakNet::BitStream* a_Data)
    {
        RakNet::MessageID messageID;
        a_Data->Read(messageID);

        switch(messageID)
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
