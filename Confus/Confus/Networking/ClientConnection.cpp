#include <iostream>
#include <vector>
#include <RakNet/BitStream.h>
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/RakPeerInterface.h>

#include "ClientConnection.h"

namespace Confus
{
    namespace Networking
    {
        ClientConnection::ClientConnection(const std::string& a_ServerIP,
            unsigned short a_Port)
        {
            m_Interface = RakNet::RakPeerInterface::GetInstance();
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
			if(m_Connected)
			m_Interface->CloseConnection(getServerAddress(), true);
            RakNet::RakPeerInterface::DestroyInstance(m_Interface);
        }

        void ClientConnection::processPackets()
        {
            RakNet::Packet* packet = m_Interface->Receive();
            while(packet != nullptr)
            {
                handlePacket(packet, static_cast<unsigned char>(packet->data[0]));

                m_Interface->DeallocatePacket(packet);
                packet = m_Interface->Receive();
            }
        }

        void ClientConnection::handlePacket(RakNet::Packet* a_Data, unsigned char a_Event)
        {
            for(size_t i = 0u; i < m_CallbackFunctionMap[a_Event].size(); i++) 
            {
                m_CallbackFunctionMap[a_Event][i](a_Data);
            }
        }

        void ClientConnection::addFunctionToMap(unsigned char a_Event, std::function<void(RakNet::Packet* a_Data)> a_Function)
        {
            m_CallbackFunctionMap[a_Event].push_back(a_Function);
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