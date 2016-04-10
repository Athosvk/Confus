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
            /// <summary>
            /// The RakNet interface for interacting with RakNet
            /// </summary>
            RakNet::RakPeerInterface* m_Interface = RakNet::RakPeerInterface::GetInstance();

        public:
            /// <summary>
            /// Initializes a new instance of the <see cref="ClientConnection"/> class.
            /// </summary>
            /// <param name="a_ServerIP">The ip address of the server to connect to.</param>
            /// <param name="a_ServerPort">The port oft the server to connect to.</param>
            ClientConnection(const std::string& a_ServerIP, unsigned short a_ServerPort = 60000);
            /// <summary>
            /// Finalizes an instance of the <see cref="ClientConnection"/> class.
            /// </summary>
            ~ClientConnection();
            /// <summary>
            /// Processes the incoming packets from the server to delegate them to the 
            /// requesting services
            /// </summary>
            void processPackets();
        private:
            unsigned short getConnectionCount() const;
            void closeServerConnection();
        };
    }
}