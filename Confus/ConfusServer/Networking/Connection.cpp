#include "Connection.h"

namespace ConfusServer
{
    namespace Networking
    {
        Connection::Connection()
        {
            RakNet::SocketDescriptor socketDescriptor(60000, nullptr);
            m_Interface->Startup(5, &socketDescriptor, 1);
        }

        Connection::~Connection()
        {
            RakNet::RakPeerInterface::DestroyInstance(m_Interface);
        }
    }
}