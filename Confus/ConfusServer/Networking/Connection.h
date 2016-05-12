#pragma once
#include <RakNet/MessageIdentifiers.h>
#include <string>
#include <queue>
#include <map>
#include <vector>
#include <functional>

namespace RakNet 
{
    class RakPeerInterface;
	struct Packet;
	class BitStream;
    struct SystemAddress;
    struct AddressOrGUID;
}

namespace ConfusServer
{
    namespace Networking
    {
        /// <summary> The type of packet </summary>
        enum class EPacketType : unsigned char
        {
            Message = 1 + ID_USER_PACKET_ENUM,
            MainPlayerJoined = 2 + ID_USER_PACKET_ENUM,
            OtherPlayerJoined = 3 + ID_USER_PACKET_ENUM,
            PlayerLeft = 4 + ID_USER_PACKET_ENUM,
            UpdatePosition = 5 + ID_USER_PACKET_ENUM,
            ScoreUpdate = 6 + ID_USER_PACKET_ENUM,
            PlayerAttack = 7 + ID_USER_PACKET_ENUM,
            MazeChange = 8 + ID_USER_PACKET_ENUM,
            UpdateHealth = 9 + ID_USER_PACKET_ENUM
        };
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
            RakNet::RakPeerInterface* m_Interface; 
            /// <summary> The map thast contains the server events and the functions that involve them. </summary>
            std::map<unsigned char, std::vector<std::function<void(RakNet::Packet* a_Data)>>> m_CallbackFunctionMap;

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
            /// <summary> Adds a function to the event in the callback function map. </summary>
            /// <param name="a_Event">The server event that should trigger the function.</param>
            /// <param name="a_Function">The function that should be added to the map.</param>
            void addFunctionToMap(unsigned char a_Event, std::function<void(RakNet::Packet* a_Data)> a_Function);
            void broadcastPacket(RakNet::BitStream* a_Stream, RakNet::AddressOrGUID* a_Excluded = nullptr);
            void sendPacket(RakNet::BitStream* a_Stream, RakNet::AddressOrGUID* a_Address);
            /// <summary> Send Package to all clients </summary>
            /// <param name="a_BitStream">The packet to send.</param>
            void broadcastBitstream(RakNet::BitStream& a_BitStream);
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
			/// <param name="a_Data">The data.</param>
            /// <param name="a_Event">The server event.</param>
            void handlePacket(RakNet::Packet* a_Data, unsigned char a_Event);
			/// <summary>
			/// Prints the InputStream.
			/// </summary>
			/// <param name="a_InputStream">The a_InputStream message.</param>
			void printMessage(RakNet::BitStream& a_InputStream);
            /// <summary> Get all the open connections in a vector </summary>
            std::vector<RakNet::SystemAddress> getOpenConnections() const;
        };
    }
}