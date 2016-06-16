#include "PlayerHandler.h"
#include "Networking/ClientConnection.h"
#include "Audio/PlayerAudioEmitter.h"
#include "../ConfusShared/PacketType.h"
#include "../ConfusShared/EHitIdentifier.h"
#include "../ConfusShared/Physics/PhysicsWorld.h"
#include "../ConfusShared/Networking/PlayerStructs.h"
#include <iostream>

namespace Confus
{
	PlayerHandler::PlayerHandler(irr::IrrlichtDevice* a_Device,
		ConfusShared::Physics::PhysicsWorld& a_PhysicsWorld, Audio::AudioManager& a_AudioManager)
		: m_PlayerNode(a_Device, a_PhysicsWorld, 1),
		m_AudioManager(a_AudioManager),
		m_PhysicsWorld(a_PhysicsWorld),
		m_Device(a_Device)
	{
	}

	PlayerHandler::PlayerConstruct::PlayerConstruct(ConfusShared::Player* a_Player, std::unique_ptr<Audio::PlayerAudioEmitter> a_AudioEmitter)
		: Player(a_Player),
		AudioEmitter(std::move(a_AudioEmitter))
	{
	}

	void PlayerHandler::setConnection(Confus::Networking::ClientConnection* a_Connection)
	{
		m_Connection = a_Connection;
		m_PlayerController = std::make_unique<LocalPlayerController>(m_PlayerNode, *m_Connection);
		m_PlayerNode.setGUID(m_Connection->getID());

		m_Connection->addFunctionToMap(static_cast<RakNet::MessageID>(ConfusShared::Networking::EPacketType::MainPlayerJoined), [this](RakNet::Packet* a_Data)
		{
			addOwnPlayer(a_Data);
		});

		m_Connection->addFunctionToMap(static_cast<RakNet::MessageID>(ConfusShared::Networking::EPacketType::PlayerLeft), [this](RakNet::Packet* a_Data)
		{
			removePlayer(a_Data);
		});

		m_Connection->addFunctionToMap(static_cast<RakNet::MessageID>(ConfusShared::Networking::EPacketType::OtherPlayerJoined), [this](RakNet::Packet* a_Data)
		{
			addOtherPlayer(a_Data);
		});

		m_Connection->addFunctionToMap(static_cast<RakNet::MessageID>(ConfusShared::Networking::EPacketType::UpdatePosition), [this](RakNet::Packet* a_Data)
		{
			updateOtherPlayer(a_Data);
		});

		m_Connection->addFunctionToMap(static_cast<RakNet::MessageID>(ConfusShared::Networking::EPacketType::UpdateHealth), [this](RakNet::Packet* a_Data)
		{
			updateHealth(a_Data);
		});
	}

	void PlayerHandler::update()
	{
		for (auto& playerPair : m_Players)
		{
			playerPair.second.Player->update();
			playerPair.second.AudioEmitter->updatePosition();
		}
        m_PlayerController->update();
	}

    ConfusShared::Player* PlayerHandler::getPlayer(const long long & a_PlayerId)
	{
        try
        {
            auto& pair = m_Players.at(a_PlayerId);
            return pair.Player;
        }
        catch(const std::out_of_range& e)
        {
            std::cerr << e.what() << std::endl;
            return nullptr;
        }
	}

	void PlayerHandler::updateOtherPlayer(RakNet::Packet* a_Data)
	{
		RakNet::BitStream bitstreamIn(a_Data->data, a_Data->length, false);
		bitstreamIn.IgnoreBytes(sizeof(RakNet::MessageID));

		for (size_t i = 0; i < m_Players.size(); ++i)
		{
			ConfusShared::Networking::Server::PlayerUpdate updateFromServer;
			bitstreamIn.Read(updateFromServer);

            auto player = getPlayer(updateFromServer.ID);
            if(player != nullptr)
            {
                player->setPosition(updateFromServer.Position);
                player->changeState(updateFromServer.State);
                if(updateFromServer.ID != m_PlayerNode.getGUID())
                {
                    player->setRotation(updateFromServer.Rotation);
                }
            }
		}
	}

	void PlayerHandler::updateHealth(RakNet::Packet* a_Data)
	{
		RakNet::BitStream inputStream(a_Data->data, a_Data->length, false);
		inputStream.IgnoreBytes(sizeof(RakNet::MessageID));

        long long id;
		inputStream.Read(id);
		auto player = getPlayer(id);

        if (player != nullptr)
        {
            int health;
            EHitIdentifier hitIdentifier;

            inputStream.Read(health);
            inputStream.Read(hitIdentifier);

            if(health > player->getHealthInstance()->getHealth())
            {
                player->getHealthInstance()->heal(health - player->getHealthInstance()->getHealth());
            }
            else if(health < player->getHealthInstance()->getHealth())
            {
                player->getHealthInstance()->damage(player->getHealthInstance()->getHealth() - health, hitIdentifier);
            }
        }
	}

	void PlayerHandler::addOwnPlayer(RakNet::Packet* a_Data)
	{
		RakNet::BitStream bitstreamIn(a_Data->data, a_Data->length, false);

		bitstreamIn.IgnoreBytes(sizeof(RakNet::MessageID));
        long long newPlayerID;
        bitstreamIn.Read(newPlayerID);
        m_PlayerNode.setGUID(newPlayerID);
		ConfusShared::ETeamIdentifier teamID;
		bitstreamIn.Read(teamID);
		m_PlayerNode.setTeamIdentifier(teamID, m_Device);

		size_t size;
		bitstreamIn.Read(size);
    	for (size_t i = 0u; i < size; i++)
		{
			ConfusShared::Networking::Server::NewPlayer playerInfo;
			bitstreamIn.Read(playerInfo);
 			ConfusShared::Player* newPlayer = new ConfusShared::Player(m_Device, m_PhysicsWorld, playerInfo.ID);
			newPlayer->setTeamIdentifier(playerInfo.Team, m_Device);
			m_Players.emplace(playerInfo.ID, PlayerConstruct(newPlayer, 
				std::make_unique<Audio::PlayerAudioEmitter>(newPlayer, &m_AudioManager)));
		}
		// Add self
		m_Players.emplace(m_PlayerNode.getGUID(), PlayerConstruct(&m_PlayerNode, std::make_unique<Audio::PlayerAudioEmitter>(&m_PlayerNode, &m_AudioManager)));
	}

	void PlayerHandler::addOtherPlayer(RakNet::Packet* a_Data)
	{
		RakNet::BitStream bitstreamIn(a_Data->data, a_Data->length, false);

		bitstreamIn.IgnoreBytes(sizeof(RakNet::MessageID));

		ConfusShared::Networking::Server::NewPlayer player;
		bitstreamIn.Read(player);   

		ConfusShared::Player* newPlayer = new ConfusShared::Player(m_Device, m_PhysicsWorld, player.ID);
		newPlayer->setTeamIdentifier(player.Team, m_Device);
		m_Players.emplace(player.ID, PlayerConstruct(newPlayer, std::make_unique<Audio::PlayerAudioEmitter>(newPlayer, &m_AudioManager)));
	}

	void PlayerHandler::removePlayer(RakNet::Packet* a_Data)
	{
		RakNet::BitStream bitstreamIn(a_Data->data, a_Data->length, false);
		bitstreamIn.IgnoreBytes(sizeof(RakNet::MessageID));

		long long id;
		bitstreamIn.Read(id);
		auto player = getPlayer(id);
        if (player != nullptr)
        {
            player->remove();
            delete(player);
            m_Players.erase(id);
        }
	}

	void PlayerHandler::handleInput(ConfusShared::EventManager* a_EventManager) const
	{
		m_PlayerController->handleInput(*a_EventManager);
	}

	ConfusShared::Player* PlayerHandler::getMainPlayer()
	{
		return &m_PlayerNode;
	}
}
