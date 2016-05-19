#include <RakNet/RakPeerInterface.h>
#include <RakNet/RakNetTypes.h>
#include <RakNet/BitStream.h>
#include <iostream>
#include <vector>
#include <string>
#include <functional>

#include "Connection.h"
#include "../../ConfusShared/Debug.h"

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
                if(packet->data[0] == ID_NEW_INCOMING_CONNECTION)
                {
                    m_Connected = true;
                }

                RakNet::BitStream inputStream(packet->data, packet->length, false);
                handlePacket(&inputStream, static_cast<unsigned char>(packet->data[0]), packet->systemAddress);
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

        void Connection::addFunctionToMap(unsigned char a_Event, std::function<void(RakNet::BitStream* a_Data, RakNet::SystemAddress& a_ClientAddress)> a_Function)
        {
            m_CallbackFunctionMap[a_Event].push_back(a_Function);
        }

        std::vector<RakNet::SystemAddress> Connection::getOpenConnections() const
        {
            auto connectionCount = getConnectionCount();
            std::vector<RakNet::SystemAddress> openConnections(static_cast<size_t>(connectionCount));
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

        void Connection::handlePacket(RakNet::BitStream* a_Data, unsigned char a_Event, RakNet::SystemAddress& a_ClientAddress)
        {
            for(size_t i = 0u; i < m_CallbackFunctionMap[a_Event].size(); i++)
            {
                m_CallbackFunctionMap[a_Event][i](a_Data, a_ClientAddress);
            }
        }

        void Connection::broadcastBitStream(RakNet::BitStream& a_BitStream)
        {
            auto openConnections = getOpenConnections();

            for(unsigned short i = 0u; i < openConnections.size(); ++i)
            {
                m_Interface->Send(&a_BitStream, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, openConnections[i], false);
            }
        }

		void Connection::sendBitStreamToClient(RakNet::BitStream& a_BitStream, RakNet::SystemAddress& a_ClientAddress)
		{
			m_Interface->Send(&a_BitStream, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, a_ClientAddress, false);
		}
    }
}