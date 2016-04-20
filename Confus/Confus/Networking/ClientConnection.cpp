#include <iostream>
#include <vector>
#include <RakNet/BitStream.h>
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/GetTime.h>

#include "ClientConnection.h"
#include "../ClientTeamScore.h"
#include "../MazeGenerator.h"

namespace Confus
{
    namespace Networking
    {
        ClientConnection::ClientConnection(const std::string& a_ServerIP,
            unsigned short a_Port)
        {
            RakNet::SocketDescriptor socketDescriptor;
            m_Interface->Startup(1, &socketDescriptor, 1);
            //There won't be a password for the server, which is why we pass nullptr
            RakNet::ConnectionAttemptResult result = 
				m_Interface->Connect(a_ServerIP.c_str(), a_Port, nullptr, 0);
            if(result != RakNet::ConnectionAttemptResult::CONNECTION_ATTEMPT_STARTED)
            {
                //For now, we throw an exception, but it would more graceful to show a message
                //to the user later on, if possible
                throw std::logic_error("Could not connect to target server, errorcode: " 
                    + std::to_string(result));
            }
        }

        ClientConnection::~ClientConnection()
        {
			//True is sent to notify the server so we can exit gracefully
			m_Interface->CloseConnection(getServerAddress(), true);
            RakNet::RakPeerInterface::DestroyInstance(m_Interface);
        }

        void ClientConnection::processPackets()
        {
            RakNet::Packet* packet = m_Interface->Receive();
            while(packet != nullptr)
            {				
                handlePacket(packet);
                m_Interface->DeallocatePacket(packet);
                packet = m_Interface->Receive();
            }
        }

        void ClientConnection::printMessage(RakNet::BitStream& a_InputStream)
        {
            RakNet::RakString contents;
            a_InputStream.IgnoreBytes(sizeof(RakNet::MessageID));
            a_InputStream.Read(contents);
            std::cout << "Message received: " << contents << std::endl;
        }


        void ClientConnection::handlePacket(RakNet::Packet* a_Packet)
        {
            switch(static_cast<unsigned char>(a_Packet->data[0]))
            {
            case static_cast<unsigned char>(ID_CONNECTION_REQUEST_ACCEPTED) :
                std::cout << "Connected to the server!\n";
                dispatchStalledMessages();
                m_Connected = true;
                break;
            case static_cast<unsigned char>(EPacketType::Message) : //or ID_MESSAGE
                printMessage(RakNet::BitStream(a_Packet->data, a_Packet->length, false));
                break;
			case static_cast<unsigned char>(EPacketType::ScoreUpdate) :
			{
				int redScore, blueScore;
				RakNet::BitStream inputStream(a_Packet->data, a_Packet->length, false);
				inputStream.IgnoreBytes(sizeof(RakNet::MessageID));
				inputStream.Read(redScore);
				inputStream.Read(blueScore);
				ClientTeamScore::RedTeamScore = redScore;
				ClientTeamScore::BlueTeamScore = blueScore;
				std::cout << "Score updated\tRed score: " << redScore << "\t Blue score: " << blueScore << std::endl;
				break;
			}
            case static_cast<unsigned char>(EPacketType::MazeChange) :
            {
                int timeMazeChanges, mazeSeed;
                RakNet::BitStream inputStream(a_Packet->data, a_Packet->length, false);
                inputStream.IgnoreBytes(sizeof(RakNet::MessageID));
                inputStream.Read(timeMazeChanges);
                inputStream.Read(mazeSeed);
                std::cout << "Update is in " << (timeMazeChanges - static_cast<int>(RakNet::GetTimeMS()))  << " ms, the seed is:\t" << mazeSeed << std::endl;
                if(MazeGeneratorReference == nullptr)
                {
                    throw std::logic_error("Maze reference generator is a nullptr");
                }
                MazeGeneratorReference->refillMainMazeRequest(mazeSeed, timeMazeChanges);
                break;
            }
            default:
                std::cout << "Message arrived with id " << static_cast<int>(a_Packet->data[0])
                    << std::endl;
            }
        }

		void ClientConnection::sendMessage(const std::string& a_Message)
		{
			if(m_Connected)
			{
				RakNet::BitStream stream;
				stream.Write(static_cast<RakNet::MessageID>(EPacketType::Message));
				stream.Write(a_Message.c_str());
				m_Interface->Send(&stream, PacketPriority::HIGH_PRIORITY,
					PacketReliability::RELIABLE_ORDERED, 0, getServerAddress(), false);
			}
			else
			{
				m_StalledMessages.push(a_Message);
			}
		}

		unsigned short ClientConnection::getConnectionCount() const
        {
            unsigned short openConnections = 0;
            m_Interface->GetConnectionList(nullptr, &openConnections);
            return openConnections;
        }

		RakNet::SystemAddress ClientConnection::getServerAddress() const
		{
			auto connectionCount = getConnectionCount();
			std::vector<RakNet::SystemAddress>
				openConnections(static_cast<size_t>(connectionCount));
			auto serverID = m_Interface->GetConnectionList(openConnections.data(),
				&connectionCount);

			if(connectionCount <= 0)
			{
				throw new std::logic_error("There is no connected server");
			}
			//We assume there is at most one connection, so it is safe to say that this
			//is the server connection
			return openConnections[0];
		}

		void ClientConnection::dispatchStalledMessages()
		{
			while(!m_StalledMessages.empty())
			{
				RakNet::BitStream stream;
				stream.Write(static_cast<RakNet::MessageID>(EPacketType::Message));
				stream.Write(m_StalledMessages.front().c_str());
				m_Interface->Send(&stream, PacketPriority::HIGH_PRIORITY,
					PacketReliability::RELIABLE_ORDERED, 0, getServerAddress(), false);
				m_StalledMessages.pop();
			}
		}
    }
}