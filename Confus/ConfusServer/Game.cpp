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

namespace ConfusServer
{
    const double Game::FixedUpdateInterval = 0.02;
    const double Game::MaxFixedUpdateInterval = 0.1;
	const double Game::ProcessPacketsInterval = 0.03;
    const double Game::MazeDelay = 2.0;
    const double Game::MazeChangeInterval = 60.0 - MazeDelay;

    Game::Game()
        : m_Device(irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_NULL)),
		m_MazeGenerator(m_Device, 41, 40, (19+20+21+22+23+24),  // magic number is just so everytime the first maze is generated it looks the same, not a specific number is chosen
			irr::core::vector2df(19., 20.), m_PhysicsWorld),
        m_BlueFlag(m_Device, ConfusShared::ETeamIdentifier::TeamBlue, m_PhysicsWorld),
        m_RedFlag(m_Device, ConfusShared::ETeamIdentifier::TeamRed, m_PhysicsWorld),
		m_TeamScoreManager(m_BlueFlag, m_RedFlag),
		m_PhysicsWorld(m_Device)
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

    void Game::resetGame() 
	{
        m_BlueFlag.returnToStartPosition();
        m_RedFlag.returnToStartPosition();
        m_TeamScoreManager.resetScore();
        m_MazeTimer = 0;
        broadcastMazeChange(19 + 20 + 21 + 22 + 23 + 24);
		
        for(ConfusShared::Player* player : m_PlayerArray) 
		{
			player->respawn();
        }
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

        for(auto player : m_PlayerArray)
        {
            player->update();
        }

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
                currentSeed = static_cast<int>(time(nullptr)) % 1000;
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

    void Game::broadcastMazeChange(int a_Seed) const
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
        std::cout << a_Data->systemAddress.ToString() << "Is the address of the player that joined";
        long long id = static_cast<long long>(a_Data->guid.g);
		ConfusShared::ETeamIdentifier teamID = m_PlayerArray.size() % 2 == 0 ? 
			ConfusShared::ETeamIdentifier::TeamRed : ConfusShared::ETeamIdentifier::TeamBlue;

        ConfusShared::Player* newPlayer = new ConfusShared::Player(m_Device, m_PhysicsWorld, id);
		newPlayer->setTeamIdentifier(teamID, m_Device);

        m_PlayerArray.push_back(newPlayer);

        RakNet::BitStream stream;
        stream.Write(static_cast<RakNet::MessageID>(Networking::EPacketType::MainPlayerJoined));
        stream.Write(static_cast<long long>(id));
        stream.Write(static_cast<ConfusShared::ETeamIdentifier>(teamID));
        stream.Write(static_cast<size_t>(m_PlayerArray.size()));
        for(size_t i = 0u; i < m_PlayerArray.size(); i++)
        {
            stream.Write(static_cast<long long>(m_PlayerArray[i]->ID));
            stream.Write(static_cast<ConfusShared::ETeamIdentifier>(m_PlayerArray[i]->getTeamIdentifier()));
        }
        RakNet::AddressOrGUID guid = a_Data->guid;
        m_Connection->sendPacket(&stream, &guid);

        RakNet::BitStream broadcastStream;
        broadcastStream.Write(static_cast<RakNet::MessageID>(Networking::EPacketType::OtherPlayerJoined));
        broadcastStream.Write(static_cast<long long>(id));
        broadcastStream.Write(static_cast<ConfusShared::ETeamIdentifier>(teamID));

        m_Connection->broadcastPacket(&broadcastStream, &guid);
		if(teamID == ConfusShared::ETeamIdentifier::TeamBlue)
		{
			newPlayer->setStartPosition(irr::core::vector3df(0.f, 10.f, 0.f));
		}
		else if(teamID == ConfusShared::ETeamIdentifier::TeamRed)
		{
			newPlayer->setStartPosition(irr::core::vector3df(0.f, 10.f, -85.f));
		}
		newPlayer->respawn();
		//Perhaps force a player update here?
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
        RakNet::BitStream stream;
        stream.Write(static_cast<RakNet::MessageID>(Networking::EPacketType::UpdatePosition));
        for(size_t i = 0u; i < m_PlayerArray.size(); ++i)
        {
            stream.Write(static_cast<long long>(m_PlayerArray[i]->ID));
            stream.Write(static_cast<irr::core::vector3df>(m_PlayerArray[i]->getPosition()));
            stream.Write(static_cast<irr::core::vector3df>(m_PlayerArray[i]->getRotation()));
        }
        m_Connection->broadcastPacket(&stream);
    }

    void Game::updateHealth(EHitIdentifier a_HitType, ConfusShared::Player* a_Player) const
    {
        RakNet::BitStream stream;
        stream.Write(static_cast<RakNet::MessageID>(Networking::EPacketType::UpdateHealth));
		stream.Write(static_cast<long long>(a_Player->ID));
		stream.Write(static_cast<int>(a_Player->getHealthInstance()->getHealth()));
		stream.Write(static_cast<EHitIdentifier>(a_HitType));
		m_Connection->broadcastPacket(&stream, nullptr);
    }
}
