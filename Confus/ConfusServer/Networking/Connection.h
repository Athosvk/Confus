#pragma once
namespace RakNet 
{
    struct Packet;
    class BitStream;
    class RakPeerInterface;
}

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
			/// <summary> The type of packet </summary>
			enum class EPacketType : unsigned char
			{
				Message = 1 + 134
			};

            /// <summary> The RakNet interface for interacting with RakNet </summary>
            RakNet::RakPeerInterface* m_Interface;// = RakNet::RakPeerInterface::GetInstance();

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
            /// <summary> Send Package to all clients </summary>
            /// <param name="a_Data">The packet to send.</param>.
            /// <param name="a_Size">The size of the data.</param>
            void sendPacketToAllClients(unsigned char* a_Data, int a_Size);
		private:
			/// <summary> Gets the amount of clients connected to this server instance </summary>
			/// <returns>The amount of clients connected</returns>
			unsigned short getConnectionCount() const;
			/// <summary>
			/// Gracefully closes all the connections to the clients this server is connected to
			/// </summary>
			void closeAllConnections();			
			/// <summary>
			/// Handles the incoming packet
			/// </summary>
			/// <param name="a_Packet">The packet.</param>
			void handlePacket(RakNet::Packet* a_Packet);			
			/// <summary>
			/// Prints the message.
			/// </summary>
			/// <param name="a_Message">The a_ message.</param>
			void printMessage(RakNet::BitStream& a_InputStream);
        };
    }
}