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
            /// <summary>
            /// The RakNet interface for interacting with RakNet
            /// </summary>
            RakNet::RakPeerInterface* m_Interface = RakNet::RakPeerInterface::GetInstance();

        public:
            /// <summary>
            /// Initializes a new instance of the <see cref="Connection"/> class.
            /// </summary>
            Connection();
            /// <summary>
            /// Finalizes an instance of the <see cref="Connection"/> class.
            /// </summary>
            ~Connection();
            /// <summary>
            /// Processes the incoming packets from the clients to delegate them to the 
            /// requesting services
            /// </summary>
            void processPackets();
        private:
            unsigned short getConnectionCount() const;
            void closeAllConnections();
        };
    }
}