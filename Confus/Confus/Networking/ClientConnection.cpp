#include "ClientConnection.h"

namespace Confus
{
    namespace Networking
    {
        ClientConnection::ClientConnection(const std::string& a_ServerIP,
            const std::string& a_ServerPort)
        {
            RakNet::SocketDescriptor socketDescriptor;
            m_Interface->Startup(1, &socketDescriptor, 1);
        }

        ClientConnection::~ClientConnection()
        {
            RakNet::RakPeerInterface::DestroyInstance(m_Interface);
        }
    }
}