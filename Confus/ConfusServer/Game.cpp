#include <Irrlicht/irrlicht.h>
#include <time.h>
#include <iostream>
#include <RakNet/BitStream.h>
#include <RakNet/GetTime.h>

#include "Game.h"
#include "../ConfusShared/Player.h"
#define DEBUG_CONSOLE
#include "../ConfusShared/Debug.h"
#include "../ConfusShared/TeamIdentifier.h"
#include "../ConfusShared/PacketType.h"
#include "../ConfusShared/Networking/PlayerStructs.h"

namespace ConfusServer
{
    const double Game::FixedUpdateInterval = 0.02;
    const double Game::MaxFixedUpdateInterval = 0.1;
	const double Game::ProcessPacketsInterval = 0.03;
    const double Game::MazeDelay = 2.0;
    const double Game::MazeChangeInterval = 60.0 - MazeDelay;

	Game::PlayerPair::PlayerPair(ConfusShared::Player* a_Player, Networking::Connection& a_Connection)
		: Player(a_Player),
		Receiver(std::make_unique<RemoteInputReceiver>(*Player, a_Connection))
	{
	}

    Game::Game()
        : m_Device(irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_NULL)),
		m_PhysicsWorld(m_Device),
        m_MazeGenerator(m_Device, 41, 40, (19+20+21+22+23+24),  // magic number is just so everytime the first maze is generated it looks the same, not a specific number is chosen
	        irr::core::vector2df(19., 20.), m_PhysicsWorld),
        m_BlueFlag(m_Device, ConfusShared::ETeamIdentifier::TeamBlue, m_PhysicsWorld),
		m_RedFlag(m_Device, ConfusShared::ETeamIdentifier::TeamRed, m_PhysicsWorld),
		m_TeamScoreManager(m_BlueFlag, m_RedFlag) 
    {
    }

    Game::~Game()
    {
        for(size_t i = 0u; i < m_Players.size(); i++)
        {
            //m_PlayerArray[i]->remove();
            //delete(m_PlayerArray[i]);
        }
    }

    void Game::resetGame() 
	{
        m_BlueFlag.returnToStartPosition();
        m_RedFlag.returnToStartPosition();
        m_TeamScoreManager.resetScore();
        m_MazeTimer = 0;
        broadcastMazeChange(19 + 20 + 21 + 22 + 23 + 24);
		
        for(auto& playerPair : m_Players) 
		{
			playerPair.second.Player->respawn();
        }
    }

	void Game::updateSceneTransformations() const
	{
		//Recurses downwwards
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

	void Game::run()
    {
        initializeConnection();


        m_TeamScoreManager.setResetCallback([this] { resetGame(); });
        auto sceneManager = m_Device->getSceneManager();
        m_LevelRootNode = m_Device->getSceneManager()->addEmptySceneNode();

        m_LevelRootNode->setPosition(irr::core::vector3df(1.0f, 1.0f, 1.0f));
        sceneManager->loadScene("Media/IrrlichtScenes/Bases2.irr", nullptr, m_LevelRootNode);
        m_LevelRootNode->setScale(irr::core::vector3df(1.0f, 1.0f, 1.0f));
		updateSceneTransformations();
		initializeLevelColliders();

        m_RedFlagUpdater = std::make_unique<RemoteFlagUpdater>(m_RedFlag, *m_Connection.get());
        m_BlueFlagUpdater = std::make_unique<RemoteFlagUpdater>(m_BlueFlag, *m_Connection.get());

        m_Connection->addFunctionToMap(ID_NEW_INCOMING_CONNECTION, [this](RakNet::Packet* a_Data)
        {
            addPlayer(a_Data);
        });

        m_Connection->addFunctionToMap(static_cast<unsigned char>(ConfusShared::Networking::EPacketType::Player), [this](RakNet::Packet* a_Packet)
        {
            RakNet::BitStream data(a_Packet->data, a_Packet->length, false);
            data.IgnoreBytes(sizeof(RakNet::MessageID));

            ConfusShared::Networking::Client::PlayerUpdate updateFromClient;
            data.Read(updateFromClient);
            m_Players.at(static_cast<long long>(a_Packet->guid.g)).Receiver->synchronize(updateFromClient);
        });

        m_Connection->addFunctionToMap(static_cast<unsigned char>(ConfusShared::Networking::EPacketType::PlayerLeft), [this](RakNet::Packet* a_Data)
        {
            removePlayer(a_Data);
        });

        m_Device->getCursorControl()->setVisible(false);
      
        while(m_Device->run())
        {
			processConnection();
            update();
            processFixedUpdates();
        }
    }

    void Game::initializeConnection()
    {
        m_Connection = std::make_unique<Networking::Connection>();
        m_TeamScoreManager.setConnection(m_Connection.get());
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

    void Game::update()
    {
        m_PreviousTicks = m_CurrentTicks;
        m_CurrentTicks = m_Device->getTimer()->getTime();
        m_DeltaTime = (m_CurrentTicks - m_PreviousTicks) / 1000.0;

        for(auto& playerPair : m_Players)
        {
            playerPair.second.Player->update();
            if(playerPair.second.Receiver->userTimedOut())
            {
                long long id = playerPair.second.Player->getGUID();
                std::cout << "[Game] Player id: " << id << " timed out." << std::endl;
                delete(playerPair.second.Player);
                auto iterator = m_Players.find(id);
				if (iterator != m_Players.end())
				{
					m_Players.erase(iterator);					
				}
            }
        }

        m_RedFlag.update();
        m_BlueFlag.update();

        static float currentDelay = 0.0f;
        static int currentSeed;
        m_MazeTimer += m_DeltaTime;
        if(m_MazeTimer >= MazeChangeInterval)
        {
            if(currentDelay == 0.0f)
            {
                currentSeed = static_cast<int>(time(nullptr)) % 1000;
                broadcastMazeChange(currentSeed);
            }
            currentDelay += static_cast<float>(m_DeltaTime);
			if (currentDelay >= MazeDelay)
			{
				m_MazeGenerator.refillMainMaze(currentSeed);
				m_MazeTimer = 0.0f;
				currentDelay = 0.0f;
			}
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

	void Game::initializeLevelColliders()
	{
		irr::core::array<irr::scene::ISceneNode*> nodes;
		m_Device->getSceneManager()->getSceneNodesFromType(irr::scene::ESNT_ANY, nodes, m_LevelRootNode);
		for(irr::u32 i = 0; i < nodes.size(); ++i)
		{
			irr::scene::ISceneNode* node = nodes[i];
			ConfusShared::Physics::Collider* collider = nullptr;

			switch(node->getType())
			{
			case irr::scene::ESNT_CUBE:
			case irr::scene::ESNT_ANIMATED_MESH:
			case irr::scene::ESNT_MESH:
				if(std::string(node->getName()).find("Ground", 0) != std::string::npos)
				{
					collider = m_PhysicsWorld.createBoxCollider(node, ConfusShared::Physics::ECollisionFilter::LevelStatic,
						ConfusShared::Physics::ECollisionFilter::Player | ConfusShared::Physics::ECollisionFilter::Interactable);
				}
				else if(std::string(node->getName()).find("Basefolder", 0) == std::string::npos)
				{
					collider = m_PhysicsWorld.createBoxCollider(node->getScale(), node, ConfusShared::Physics::ECollisionFilter::LevelStatic |
						ConfusShared::Physics::ECollisionFilter::Interactable,
						ConfusShared::Physics::ECollisionFilter::Player);
				}
				if(collider != nullptr)
				{
					collider->getRigidBody()->makeStatic();
				}
				break;
			default:
				break;
			}
		}
	}

	void Game::fixedUpdate()
    {
        updatePlayers();
		m_MazeGenerator.fixedUpdate();
		m_PhysicsWorld.stepSimulation(static_cast<float>(FixedUpdateInterval));
        m_RedFlagUpdater->fixedUpdate();
        m_BlueFlagUpdater->fixedUpdate();
	}

    void Game::broadcastMazeChange(int a_Seed) const
    {
        int newTime = static_cast<int>(RakNet::GetTimeMS()) + (static_cast<int>(MazeDelay * 1000));

        RakNet::BitStream bitStream;
        bitStream.Write(static_cast<RakNet::MessageID>(ConfusShared::Networking::EPacketType::MazeChange));
        bitStream.Write(newTime);
        bitStream.Write(a_Seed);
        m_Connection->broadcastBitstream(bitStream);
    }

    void Game::addPlayer(RakNet::Packet* a_Data)
    {
        long long id = static_cast<long long>(a_Data->guid.g);
		ConfusShared::ETeamIdentifier teamID = m_Players.size() % 2 == 0 ? 
			ConfusShared::ETeamIdentifier::TeamRed : ConfusShared::ETeamIdentifier::TeamBlue;

        ConfusShared::Player* newPlayer = new ConfusShared::Player(m_Device, m_PhysicsWorld, id);
		newPlayer->setTeamIdentifier(teamID, m_Device);
		if(teamID == ConfusShared::ETeamIdentifier::TeamBlue)
		{
			newPlayer->setStartPosition(irr::core::vector3df(0.f, 10.f, 0.f));
		}
		else if(teamID == ConfusShared::ETeamIdentifier::TeamRed)
		{
			newPlayer->setStartPosition(irr::core::vector3df(0.f, 10.f, -85.f));
		}
		newPlayer->respawn();

        RakNet::BitStream stream;
        stream.Write(static_cast<RakNet::MessageID>(ConfusShared::Networking::EPacketType::MainPlayerJoined));
        stream.Write(static_cast<ConfusShared::ETeamIdentifier>(teamID));
        stream.Write(m_Players.size());
        for(auto& playerPair : m_Players)
        {
			ConfusShared::Networking::Server::NewPlayer playerInfo(playerPair.second.Player);
			stream.Write(playerInfo);
        }
        RakNet::AddressOrGUID guid = a_Data->guid;
        m_Connection->sendPacket(&stream, &guid);

		ConfusShared::Networking::Server::NewPlayer playerInfo(newPlayer);
        RakNet::BitStream broadcastStream;
		broadcastStream.Write(static_cast<RakNet::MessageID>(ConfusShared::Networking::EPacketType::OtherPlayerJoined));
        broadcastStream.Write(playerInfo);
        m_Connection->broadcastPacket(&broadcastStream, &guid);

		m_Players.emplace(newPlayer->getGUID(), PlayerPair(newPlayer, *m_Connection));
        std::cout << "[Game] Player id: " << id << " joined." << std::endl;
    }

    void Game::removePlayer(RakNet::Packet* a_Data)
    {
        long long id = static_cast<long long>(a_Data->guid.g);
        std::cout << "[Game] Player id: " << id << " left." << std::endl;

		PlayerPair& playerPair = m_Players.at(id);
		playerPair.Player->remove();
		delete(playerPair.Player);
        auto iterator = m_Players.find(id);
        m_Players.erase(iterator);

        RakNet::BitStream stream;
        stream.Write(static_cast<RakNet::MessageID>(ConfusShared::Networking::EPacketType::PlayerLeft));
        stream.Write(id);
        RakNet::AddressOrGUID guid = a_Data->guid;
        m_Connection->broadcastPacket(&stream, &guid);
    }

    void Game::updatePlayers()
    {
        RakNet::BitStream stream;
        stream.Write(static_cast<RakNet::MessageID>(ConfusShared::Networking::EPacketType::UpdatePosition));
        for(auto& playerPair : m_Players)
        {
			ConfusShared::Networking::Server::PlayerUpdate playerUpdateFromServer;
			playerUpdateFromServer.ID = playerPair.second.Player->getGUID();
			playerUpdateFromServer.Position = playerPair.second.Player->getPosition();
			playerUpdateFromServer.Rotation = playerPair.second.Player->getRotation();
			playerUpdateFromServer.State = playerPair.second.Player->getState();
			stream.Write(playerUpdateFromServer);
        }
        m_Connection->broadcastPacket(&stream);
    }

    void Game::updateHealth(EHitIdentifier a_HitType, ConfusShared::Player* a_Player) const
    {
        RakNet::BitStream stream;
        stream.Write(static_cast<RakNet::MessageID>(ConfusShared::Networking::EPacketType::UpdateHealth));
		stream.Write(static_cast<long long>(a_Player->getGUID()));
		stream.Write(static_cast<int>(a_Player->getHealthInstance()->getHealth()));
		stream.Write(static_cast<EHitIdentifier>(a_HitType));
		m_Connection->broadcastPacket(&stream, nullptr);
    }
}
