#include <RakNet/RakPeerInterface.h>
#include <RakNet/RakNetTypes.h>
#include <RakNet/BitStream.h>
#include <iostream>
#include <vector>
#include <string>

#include "Connection.h"
#define DEBUG_CONSOLE
#include "../../Common/Debug.h"

namespace ConfusServer
{
    namespace Networking
    {
        Connection::Connection()
        {
            m_Interface = RakNet::RakPeerInterface::GetInstance();
            RakNet::SocketDescriptor socketDescriptor(60000, nullptr);
            auto result = m_Interface->Startup(5, &socketDescriptor, 1);
			if(result != RakNet::StartupResult::RAKNET_STARTED)
			{
				throw std::logic_error("Could not start RakNet, errorcode " +
					std::to_string(result));
			}
            //Use 10 as a general case for a 5 vs 5 matchup
            m_Interface->SetMaximumIncomingConnections(10);
        }

        Connection::~Connection()
        {
            closeAllConnections();
            RakNet::RakPeerInterface::DestroyInstance(m_Interface);
        }

        void Connection::processPackets()
        {
            RakNet::Packet* packet = m_Interface->Receive();
            while(packet != nullptr)
            {
				handlePacket(packet);
                m_Interface->DeallocatePacket(packet);
                packet = m_Interface->Receive();
            }
        }

        std::vector<RakNet::SystemAddress> Connection::getOpenConnections()
        {
            auto connectionCount = getConnectionCount();
            std::vector<RakNet::SystemAddress>
                openConnections(static_cast<size_t>(connectionCount));
            auto serverID = m_Interface->GetConnectionList(openConnections.data(),
                &connectionCount);
            return openConnections;
        }

        unsigned short Connection::getConnectionCount() const
        {
            unsigned short openConnections = 0;
            //Passing nullptr allows us to get the amount of open connections
            m_Interface->GetConnectionList(nullptr, &openConnections);
            return openConnections;
        }

        void Connection::closeAllConnections()
        {
            auto openConnections = getOpenConnections();

            for(unsigned short i = 0u; i < openConnections.size(); ++i)
            {
                m_Interface->CloseConnection(openConnections[i], true);
            }
        }

		void Connection::handlePacket(RakNet::Packet* a_Packet)
		{
			switch(static_cast<unsigned char>(a_Packet->data[0]))
			{
			case static_cast<unsigned char>(EPacketType::Message) :
				printMessage(RakNet::BitStream(a_Packet->data, a_Packet->length, false));
				break;
			default:
				std::cout << "Message arrived with id " << static_cast<int>(a_Packet->data[0])
					<< std::endl;
			}
		}

		void Connection::printMessage(RakNet::BitStream& a_InputStream)
		{
			RakNet::RakString contents;
			a_InputStream.IgnoreBytes(sizeof(RakNet::MessageID));
			a_InputStream.Read(contents);
			std::cout << "Message received: " << contents << std::endl;
		}

        void Connection::broadcastBitstream(RakNet::BitStream& a_BitStream)
        {
            auto openConnections = getOpenConnections();

            for(unsigned short i = 0u; i < openConnections.size(); ++i)
            {
                m_Interface->Send(&a_BitStream, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, openConnections[i], false);
            }
        }
    }
}