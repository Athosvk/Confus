#pragma once
#include <RakNet/RakPeerInterface.h>

namespace ConfusServer
{
    namespace Networking
    {
        /// <summary>
        /// Represents the outgoing connection/group of outgoing connections to the client(s)
        /// that the server instance will be receiving packets from and sending packets to
		/// </summary>
        /// <remarks> 
        /// This header must be included before Windows.h is ever included, due to the nature
        /// of the operating system libraries. This may form an exception to the coding
        /// guidelines that state the order of file inclusion
        /// </remarks>
        class Connection
        {
        private:
            /// <summary> The RakNet interface for interacting with RakNet </summary>
            RakNet::RakPeerInterface* m_Interface = RakNet::RakPeerInterface::GetInstance();

        public:
            /// <summary> Initializes a new instance of the <see cref="Connection"/> class. </summary>
            Connection();
            /// <summary> Finalizes an instance of the <see cref="Connection"/> class. </summary>
            ~Connection();
            /// <summary>
            /// Processes the incoming packets from the clients to delegate them to the 
            /// requesting services
            /// </summary>
            void processPackets();
		private:
			/// <summary> Gets the amount of clients connected to this server instance </summary>
			/// <returns>The amount of clients connected</returns>
			unsigned short getConnectionCount() const;
			/// <summary>
			/// Gracefully closes all the connections to the clients this server is connected to
			/// </summary>
			void closeAllConnections();
        };
    }
}