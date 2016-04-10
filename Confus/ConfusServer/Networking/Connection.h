#pragma once
#include <RakNet/RakPeerInterface.h>

namespace ConfusServer
{
    namespace Networking
    {
        /// <summary>
        /// Represents the outgoing connection/group of outgoing connections to the client(s)
        /// that the server instance will be receiving packets from and sending packets to
        /// <remarks> 
        /// This header must be included before Windows.h is ever included, due to the nature
        /// of the operating system libraries. This may form an exception to the coding
        /// guidelines that state the order of file inclusion
        /// </remarks>
        /// </summary>
        class Connection
        {
        private:
            RakNet::RakPeerInterface* m_Interface = RakNet::RakPeerInterface::GetInstance();

        public:
            Connection();
            ~Connection();
        };
    }
}