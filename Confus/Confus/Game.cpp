#include <Irrlicht/irrlicht.h>
#include <time.h>
#include <iostream>
#include <RakNet/BitStream.h>
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/GetTime.h>
#include <chrono>
#include <thread>

#include "Game.h"
#include "Player.h"
#include "Flag.h"
#include "FlagGUI.h"
#include "WinScreen.h"

#define DEBUG_CONSOLE
#include "../Common/Debug.h"
#include "../Common/TeamIdentifier.h"


namespace Confus
{
    Game::Game(irr::IrrlichtDevice* a_Device) : BaseGame(a_Device),
		m_MazeGenerator(m_Device,40,40, irr::core::vector3df(0.0f, 0.0f, 0.0f),(19+20+21+22+23+24), irr::core::vector2df(30.,30.)), // magic number is just so everytime the first maze is generated it looks the same, not a specific number is chosen
        m_PlayerNode(m_Device, 1, ETeamIdentifier::TeamBlue, true),
        m_BlueFlag(m_Device, ETeamIdentifier::TeamBlue),
        m_RedFlag(m_Device, ETeamIdentifier::TeamRed),
        m_RedRespawnFloor(m_Device),
        m_BlueRespawnFloor(m_Device),
		m_GUI(m_Device, &m_PlayerNode)
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
    }

    Game::~Game()
    {
        for(size_t i = 0u; i < m_PlayerArray.size(); i++)
        {
            //m_PlayerArray[i]->remove();
            //delete(m_PlayerArray[i]);
        }
    }

    void Game::start()
    {
        m_Device->setWindowCaption(L"Game");

        initializeConnection();
        auto sceneManager = m_Device->getSceneManager();
        m_LevelRootNode = m_Device->getSceneManager()->addEmptySceneNode();

        m_LevelRootNode->setPosition(irr::core::vector3df(1.0f, 1.0f, 1.0f));
        sceneManager->loadScene("Media/IrrlichtScenes/Bases2.irr", nullptr, m_LevelRootNode);
        m_LevelRootNode->setScale(irr::core::vector3df(1.0f, 1.0f, 1.0f));
        m_LevelRootNode->setVisible(true);
        m_Device->setEventReceiver(&m_EventManager);

        processTriangleSelectors();

        m_PlayerNode.setLevelCollider(m_Device->getSceneManager(), m_LevelRootNode->getTriangleSelector());
        m_BlueFlag.setCollisionTriangleSelector(m_Device->getSceneManager(), m_LevelRootNode->getTriangleSelector());
        m_RedFlag.setCollisionTriangleSelector(m_Device->getSceneManager(), m_LevelRootNode->getTriangleSelector());

        m_BlueRespawnFloor.setPosition(irr::core::vector3df(0.f, 3.45f, 11.f));
        m_RedRespawnFloor.setPosition(irr::core::vector3df(0.f, 3.45f, -83.f));

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
    }

    void Game::processTriangleSelectors()
    {
        auto sceneManager = m_Device->getSceneManager();
        auto metatriangleSelector = sceneManager->createMetaTriangleSelector();
        
        irr::core::array<irr::scene::ISceneNode*> nodes;
        sceneManager->getSceneNodesFromType(irr::scene::ESNT_ANY, nodes);
        for(irr::u32 i = 0; i < nodes.size(); ++i)
        {
            irr::scene::ISceneNode* node = nodes[i];
            irr::scene::ITriangleSelector* selector = nullptr;

            switch(node->getType())
            {
            case irr::scene::ESNT_CUBE:
            case irr::scene::ESNT_ANIMATED_MESH:
                selector = m_Device->getSceneManager()->createTriangleSelectorFromBoundingBox(node);
                break;
            case irr::scene::ESNT_MESH:
            case irr::scene::ESNT_SPHERE:
                selector = sceneManager->createTriangleSelector(((irr::scene::IMeshSceneNode*)node)->getMesh(), node);
                break;
            case irr::scene::ESNT_TERRAIN:
                selector = sceneManager->createTerrainTriangleSelector((irr::scene::ITerrainSceneNode*)node);
                break;
            case irr::scene::ESNT_OCTREE:
                selector = sceneManager->createOctreeTriangleSelector(((irr::scene::IMeshSceneNode*)node)->getMesh(), node);
                break;
            default:
                break;
            }
            
            if(selector)
            {
                metatriangleSelector->addTriangleSelector(selector);
                selector->drop();
            }
        }
        m_LevelRootNode->setTriangleSelector(metatriangleSelector);
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
        m_Connection->addFunctionToMap(static_cast<unsigned char>(Networking::EPacketType::MazeChange), [this](RakNet::Packet* a_Packet) 
        {
            int timeMazeChanges, mazeSeed;
            RakNet::BitStream inputStream(a_Packet->data, a_Packet->length, false);
            inputStream.IgnoreBytes(sizeof(RakNet::MessageID));
            inputStream.Read(timeMazeChanges);
            inputStream.Read(mazeSeed);
            std::cout << "Update is in " << (timeMazeChanges - static_cast<int>(RakNet::GetTimeMS())) << " ms, the seed is:\t" << mazeSeed << std::endl;
            m_MazeGenerator.refillMainMazeRequest(mazeSeed, timeMazeChanges);
        });

        m_Connection->addFunctionToMap(static_cast<unsigned char>(Networking::EPacketType::ScoreUpdate), [](RakNet::Packet* a_Packet) 
        {
            int redScore, blueScore;
            RakNet::BitStream inputStream(a_Packet->data, a_Packet->length, false);
            inputStream.IgnoreBytes(sizeof(RakNet::MessageID));
            inputStream.Read(redScore);
            inputStream.Read(blueScore);
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

            WinScreen winScreen(m_Device, a_TeamIdentifier);
            winScreen.run();
            m_Device->setWindowCaption(L"Game");
            m_Device->setEventReceiver(&m_EventManager);

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
        m_PlayerNode.handleInput(m_EventManager);
    }

    void Game::update()
    {
        m_Connection->processPackets();
        handleInput();

        m_PlayerNode.update();
		m_GUI.update();
        m_Listener.setPosition(m_PlayerNode.CameraNode->getAbsolutePosition());
        irr::core::quaternion playerRotation(m_PlayerNode.CameraNode->getRotation());

        //Todo: Fix rotations
        irr::core::vector3df upVector = playerRotation * irr::core::vector3df( 0, 1, 0 );
        irr::core::vector3df forwardVector = playerRotation * irr::core::vector3df(0, 0, 1);
        m_Listener.setDirection(forwardVector, upVector);    

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
        static float timer = 0.0f;
        timer += static_cast<float>(m_DeltaTime);
        if(timer >= 3.0f && timer <= 8.0f)
        {
            m_BlueRespawnFloor.enableCollision();
            m_RedRespawnFloor.enableCollision();
        }
        if(timer >= 9.0f)
        {
            timer = 0.0f;
            m_BlueRespawnFloor.disableCollision();
            m_RedRespawnFloor.disableCollision();
        }
        m_MazeGenerator.fixedUpdate();
    }

    void Game::end()
    {
    }

    void Game::updateOtherPlayer(RakNet::Packet* a_Data)
    {
        RakNet::BitStream inputStream(a_Data->data, a_Data->length, false);
        inputStream.IgnoreBytes(sizeof(RakNet::MessageID));

        long long id;

        inputStream.Read(id);

        for(size_t i = 0u; i < m_PlayerArray.size(); i++)
        {
            if(m_PlayerArray[i]->MainPlayer == false && m_PlayerArray[i]->ID == id)
            {
                irr::core::vector3df pos;
                irr::core::vector3df rot;

                inputStream.Read(pos);
                inputStream.Read(rot);

                m_PlayerArray[i]->setPosition(pos);
                m_PlayerArray[i]->setRotation(rot);
                break;
            }
        }
    }


    //need to test of the guid.g is the right one, and not the one from the server
    void Game::addOwnPlayer(RakNet::Packet* a_Data)
    {
        RakNet::BitStream inputStream(a_Data->data, a_Data->length, false);
        inputStream.IgnoreBytes(sizeof(RakNet::MessageID));

        inputStream.Read(m_PlayerNode.ID);
        inputStream.Read(m_PlayerNode.TeamIdentifier);
        m_PlayerNode.respawn();
        m_PlayerNode.updateColor(m_Device);

        size_t size;
        inputStream.Read(size);

        for(size_t i = 0u; i < size; i++)
        {
            long long id;
            ETeamIdentifier teamID;

            inputStream.Read(id);
            inputStream.Read(teamID);

            Player* newPlayer = new Player(m_Device, id, teamID, false);
            
            newPlayer->setLevelCollider(m_Device->getSceneManager(), m_LevelRootNode->getTriangleSelector());
            m_PlayerArray.push_back(newPlayer);
        }

        m_PlayerArray.push_back(&m_PlayerNode);
    }

    void Game::addOtherPlayer(RakNet::Packet* a_Data)
    {
        RakNet::BitStream inputStream(a_Data->data, a_Data->length, false);
        inputStream.IgnoreBytes(sizeof(RakNet::MessageID));

        long long id;
        ETeamIdentifier teamID;

        inputStream.Read(id);
        inputStream.Read(teamID);

        Player* newPlayer = new Player(m_Device, id, teamID, false);
        newPlayer->setLevelCollider(m_Device->getSceneManager(), m_LevelRootNode->getTriangleSelector());
        m_PlayerArray.push_back(newPlayer);
    }

    void Game::removePlayer(RakNet::Packet* a_Data)
    {
        RakNet::BitStream inputStream(a_Data->data, a_Data->length, false);
        inputStream.IgnoreBytes(sizeof(RakNet::MessageID));

        long long id;

        inputStream.Read(id);

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
    }
}