#include <iostream>
#include <vector>
#include <string>
#include <RakNet/BitStream.h>

#include "Connection.h"

namespace ConfusServer
{
    namespace Networking
    {
        Connection::Connection(Player* a_MainPlayer)
        {
            RakNet::SocketDescriptor socketDescriptor(60000, nullptr);
            auto result = m_Interface->Startup(5, &socketDescriptor, 1);
			if(result != RakNet::StartupResult::RAKNET_STARTED)
			{
				throw std::logic_error("Could not start RakNet, errorcode " +
					std::to_string(result));
			}
            //Use 10 as a general case for a 5 vs 5 matchup
            m_Interface->SetMaximumIncomingConnections(10);
            m_MainPlayer = a_MainPlayer;
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

        unsigned short Connection::getConnectionCount() const
        {
            unsigned short openConnections = 0;
			//Passing nullptr allows us to get the amount of open connections
            m_Interface->GetConnectionList(nullptr, &openConnections);
            return openConnections;
        }

        void Connection::closeAllConnections()
        {
            auto connectionCount = getConnectionCount();
            std::vector<RakNet::SystemAddress>
                openConnections(static_cast<size_t>(connectionCount));
            auto serverID = m_Interface->GetConnectionList(openConnections.data(),
                &connectionCount);

            for(unsigned short i = 0u; i < connectionCount; ++i)
            {
                m_Interface->CloseConnection(openConnections[i], true);
            }
        }

		void Connection::handlePacket(RakNet::Packet* a_Packet)
		{
            switch(static_cast<unsigned char>(a_Packet->data[0]))
			{
            // Player attack ID.
            case (ID_USER_PACKET_ENUM + 3) :
            {
                RakNet::BitStream inputStream(a_Packet->data, a_Packet->length, false);
                
                bool isHeavyAttack;
                inputStream.IgnoreBytes(sizeof(RakNet::MessageID));
                inputStream.Read(isHeavyAttack);
                if(isHeavyAttack)
                {
                    m_MainPlayer->startHeavyAttack();
                } 
                else
                {
                    m_MainPlayer->startLightAttack();
                }
                #ifdef DEBUG_CONSOLE
                std::cout << "Player pressed attack button\n";
                #endif
            }
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
    }
}