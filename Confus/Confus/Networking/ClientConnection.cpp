#include <iostream>
#include <vector>

#include "ClientConnection.h"

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
            RakNet::ConnectionAttemptResult result = m_Interface->Connect(a_ServerIP.c_str(), a_Port, nullptr, 0);
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
            closeServerConnection();
            RakNet::RakPeerInterface::DestroyInstance(m_Interface);
        }

        void ClientConnection::processPackets()
        {
            RakNet::Packet* packet = m_Interface->Receive();
            while(packet != nullptr)
            {
                std::cout << "Message has arrived, identifier: " << std::endl;
                m_Interface->DeallocatePacket(packet);
                packet = m_Interface->Receive();
            }
        }

        unsigned short ClientConnection::getConnectionCount() const
        {
            unsigned short openConnections = 0;
            m_Interface->GetConnectionList(nullptr, &openConnections);
            return openConnections;
        }

        void ClientConnection::closeServerConnection()
        {
            auto connectionCount = getConnectionCount();
            std::vector<RakNet::SystemAddress>
                openConnections(static_cast<size_t>(connectionCount));
            auto serverID = m_Interface->GetConnectionList(openConnections.data(),
                &connectionCount);

            if(connectionCount > 0)
            {
                //We assume there is at most one connection
                m_Interface->CloseConnection(openConnections[0], true);
            }
        }
    }
}