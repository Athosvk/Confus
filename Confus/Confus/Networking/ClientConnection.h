#pragma once
#include <RakNet/RakPeerInterface.h>
#include <string>

namespace Confus
{
    namespace Networking
    {
        /// <summary>
        /// Represents a connection to the server that this clients is connected to.
        /// A game should only have one of these, since a client will only connect to one
        /// server at a time. The networking library also does not allow for multiple
        /// instances, either.
        /// <remarks> 
        /// This header must be included before Windows.h is ever included, due to the nature
        /// of the operating system libraries. This may form an exception to the coding
        /// guidelines that state the order of file inclusion
        /// </remarks>
        /// </summary>
        class ClientConnection
        {
        private:
            RakNet::RakPeerInterface* m_Interface = RakNet::RakPeerInterface::GetInstance();

        public:
            ClientConnection(const std::string& a_ServerIP, const std::string& a_ServerPort);
            ~ClientConnection();
        };
    }
}