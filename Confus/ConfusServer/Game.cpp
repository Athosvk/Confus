#include <Irrlicht/irrlicht.h>
#include <time.h>
#include <iostream>
#include <RakNet\GetTime.h>

#define DEBUG_CONSOLE
#include "Game.h"
#include "Player.h"
#include "Flag.h"
#include "../ConfusShared/Debug.h"
#include "../ConfusShared/TeamIdentifier.h"

namespace ConfusServer
{
    const double Game::FixedUpdateInterval = 0.02;
    const double Game::MaxFixedUpdateInterval = 0.1;
	const double Game::ProcessPacketsInterval = 0.03;
    const double Game::MazeDelay = 2.0;
    const double Game::MazeChangeInterval = 60.0 - MazeDelay;

    Game::Game()
        : m_Device(irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_NULL)),
		m_MazeGenerator(m_Device, irr::core::vector3df(0.0f, 0.0f, 0.0f),(19+20+21+22+23+24)), // magic number is just so everytime the first maze is generated it looks the same, not a specific number is chosen
        m_PlayerNode(m_Device, 1, ETeamIdentifier::TeamRed, true),        
        m_SecondPlayerNode(m_Device, 1, ETeamIdentifier::TeamRed, false),
        m_BlueFlag(m_Device, ETeamIdentifier::TeamBlue, &m_TeamScoreManager),
        m_RedFlag(m_Device, ETeamIdentifier::TeamRed, &m_TeamScoreManager)
    {
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

        processTriangleSelectors();

        m_PlayerNode.setLevelCollider(m_Device->getSceneManager(), m_LevelRootNode->getTriangleSelector());
        m_SecondPlayerNode.setLevelCollider(m_Device->getSceneManager(), m_LevelRootNode->getTriangleSelector());
        m_BlueFlag.setCollisionTriangleSelector(m_Device->getSceneManager(), m_LevelRootNode->getTriangleSelector());
        m_RedFlag.setCollisionTriangleSelector(m_Device->getSceneManager(), m_LevelRootNode->getTriangleSelector());

        m_Connection->addFunctionToMap(ID_NEW_INCOMING_CONNECTION, [this](RakNet::Packet* a_Data)
        {
            addPlayer(a_Data);
        });

        m_Connection->addFunctionToMap(ID_DISCONNECTION_NOTIFICATION, [this](RakNet::Packet* a_Data)
        {
            removePlayer(a_Data);
        });


        m_Device->getCursorControl()->setVisible(false);
      
        while(m_Device->run())
        {
			processConnection();
            handleInput();
            update();
            processFixedUpdates();
            render();
        }
    }

    void Game::initializeConnection()
    {
        m_Connection = std::make_unique<Networking::Connection>();
        m_TeamScoreManager.setConnection(m_Connection.get());
        m_RedFlag.setConnection(m_Connection.get());
        m_BlueFlag.setConnection(m_Connection.get());
    }

	void Game::processConnection()
	{
		m_ConnectionUpdateTimer += m_DeltaTime;
		if (m_ConnectionUpdateTimer >= ProcessPacketsInterval)
		{
			m_ConnectionUpdateTimer = 0;
			m_Connection->processPackets();
		}
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
            node->setDebugDataVisible(irr::scene::EDS_BBOX_ALL);

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
        m_BlueFlag.update();
        m_RedFlag.update();

        m_Listener.setPosition(m_PlayerNode.CameraNode->getAbsolutePosition());

        irr::core::quaternion playerRotation(m_PlayerNode.CameraNode->getRotation());
        irr::core::vector3df upVector = playerRotation * irr::core::vector3df( 0, 1, 0 );
        irr::core::vector3df forwardVector = playerRotation * irr::core::vector3df(0, 0, 1);
        m_Listener.setDirection(forwardVector, upVector);

        static float currentDelay = 0.0f;
        static int currentSeed;
        m_MazeTimer += m_DeltaTime;
        if(m_MazeTimer >= MazeChangeInterval)
        {
            if(currentDelay >= MazeDelay)
            {
                m_MazeGenerator.refillMainMaze(currentSeed);
                m_MazeTimer = 0.0f;
                currentDelay = 0.0f;
            }
            if(currentDelay == 0.0f)
            {
                currentSeed = static_cast<int>(time(0)) % 1000;
                broadcastMazeChange(currentSeed);
            }
            currentDelay += static_cast<float>(m_DeltaTime);
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
        updatePlayers();
		m_MazeGenerator.fixedUpdate();
    }

    void Game::broadcastMazeChange(int a_Seed)
    {
        int newTime = static_cast<int>(RakNet::GetTimeMS()) + (static_cast<int>(MazeDelay * 1000));

        RakNet::BitStream bitStream;
        bitStream.Write(static_cast<RakNet::MessageID>(Networking::EPacketType::MazeChange));
        bitStream.Write(newTime);
        bitStream.Write(a_Seed);
        m_Connection->broadcastBitstream(bitStream);
    }

    void Game::addPlayer(RakNet::Packet* a_Data)
    {
        long long id = static_cast<long long>(a_Data->guid.g);
        ETeamIdentifier teamID = m_PlayerArray.size() % 2 == 0 ? ETeamIdentifier::TeamRed : ETeamIdentifier::TeamBlue;

        Player* newPlayer = new Player(m_Device, id, teamID, false);
        m_PlayerArray.push_back(newPlayer);

        RakNet::BitStream stream;
        stream.Write(static_cast<RakNet::MessageID>(Networking::EPacketType::MainPlayerJoined));
        stream.Write(static_cast<long long>(id));
        stream.Write(static_cast<ETeamIdentifier>(teamID));
        stream.Write(static_cast<size_t>(m_PlayerArray.size()));
        for(size_t i = 0u; i < m_PlayerArray.size(); i++)
        {
            stream.Write(static_cast<long long>(m_PlayerArray[i]->ID));
            stream.Write(static_cast<ETeamIdentifier>(m_PlayerArray[i]->TeamIdentifier));
        }
        RakNet::AddressOrGUID guid = a_Data->guid;
        m_Connection->sendPacket(&stream, &guid);

        RakNet::BitStream broadcastStream;
        broadcastStream.Write(static_cast<RakNet::MessageID>(Networking::EPacketType::OtherPlayerJoined));
        broadcastStream.Write(static_cast<long long>(id));
        broadcastStream.Write(static_cast<ETeamIdentifier>(teamID));

        m_Connection->broadcastPacket(&broadcastStream, &guid);

        std::cout << id << " joined" << std::endl;
    }

    void Game::removePlayer(RakNet::Packet* a_Data)
    {
        long long id = static_cast<long long>(a_Data->guid.g);
        std::cout << id << " left" << std::endl;

        for(size_t i = 0u; i < m_PlayerArray.size(); i++)
        {
            if(m_PlayerArray[i]->ID == id)
            {
                m_PlayerArray[i]->remove();
                delete(m_PlayerArray[i]);
                m_PlayerArray.erase(m_PlayerArray.begin()+i);
            }
        }

        RakNet::BitStream stream;
        stream.Write(static_cast<RakNet::MessageID>(Networking::EPacketType::PlayerLeft));
        stream.Write(id);
        RakNet::AddressOrGUID guid = a_Data->guid;
        m_Connection->broadcastPacket(&stream, &guid);
    }

    void Game::updatePlayers()
    {
        for(size_t i = 0u; i < m_PlayerArray.size(); i++)
        {
            RakNet::BitStream stream;
            stream.Write(static_cast<RakNet::MessageID>(Networking::EPacketType::UpdatePosition));
            stream.Write(static_cast<long long>(m_PlayerArray[i]->ID));
            stream.Write(static_cast<irr::core::vector3df>(m_PlayerArray[i]->CameraNode->getPosition()));
            stream.Write(static_cast<irr::core::vector3df>(m_PlayerArray[i]->CameraNode->getRotation()));
            m_Connection->broadcastPacket(&stream, nullptr);
        }
    }

    void Game::render()
    {
        m_Device->getVideoDriver()->beginScene(true, true, irr::video::SColor(255, 100, 101, 140));
        m_Device->getSceneManager()->drawAll();
        //m_Device->getGUIEnvironment()->drawAll();
        m_Device->getVideoDriver()->endScene();
    }
}