#pragma once
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/BitStream.h>
#include <string>
#include <queue>
#include <map>
#include <vector>
#include <functional>

namespace RakNet
{
    class RakPeerInterface;
    struct SystemAddress;
    struct Packet;
    class BitStream;
    struct AddressOrGUID;
}

enum PacketReliability;

namespace Confus
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
			Player = 9 + ID_USER_PACKET_ENUM,
            EndOfGame = 11 + ID_USER_PACKET_ENUM,
            UpdateHealth = 10 + ID_USER_PACKET_ENUM
        };
        /// <summary>
        /// Represents a connection to the server that this clients is connected to.
        /// A game should only have one of these, since a client will only connect to one
        /// server at a time. The networking library also does not allow for multiple
        /// instances, either.
		/// </summary>
        /// <remarks> 
        /// This header must be included before Windows.h is ever included, due to the nature
        /// of the operating system libraries. This may form an exception to the coding
        /// guidelines that state the order of file inclusion
        /// </remarks>
        class ClientConnection
        {
		public:
			char ClientID = 0;
		private:
			/// <summary> Whether we are connected to a server</summary>
			bool m_Connected = false;
            /// <summary> The map thast contains the server events and the functions that involve them. </summary>
            std::map<unsigned char, std::vector<std::function<void(RakNet::BitStream* a_Data)>>> m_CallbackFunctionMap;
        public:           
            /// <summary> The RakNet interface for interacting with RakNet </summary>
            RakNet::RakPeerInterface* m_Interface;
            /// <summary> Initializes a new instance of the <see cref="ClientConnection"/> class. </summary>
            /// <param name="a_ServerIP">The ip address of the server to connect to.</param>
            /// <param name="a_ServerPort">The port oft the server to connect to.</param>
            ClientConnection(const std::string& a_ServerIP, unsigned short a_ServerPort = 60000);
            /// <summary> Finalizes an instance of the <see cref="ClientConnection"/> class. </summary>
            ~ClientConnection();
            /// <summary>
            /// Processes the incoming packets from the server to delegate them to the 
            /// requesting services
            /// </summary>
			void processPackets();
			/// <summary>
			/// Sends a message to the server
			/// </summary>
			/// <param name="a_Stream">The Bitstream that needs to be send.</param>
			void sendMessage(RakNet::BitStream* a_Stream, PacketReliability a_Reliability);
            /// <summary> Adds a function to the event in the callback function map. </summary>
            /// <param name="a_Event">The server event that should trigger the function.</param>
            /// <param name="a_Function">The function that should be added to the map.</param>
            void addFunctionToMap(unsigned char a_Event, std::function<void(RakNet::BitStream* a_Data)> a_Function);
		private:
			/// <summary> Gets the amount of clients connected to this server instance </summary>
			/// <returns> The amount of clients connected </returns>
			unsigned short getConnectionCount() const;
			/// <summary> Closes the connection to the server gracefully </summary>
			/// <remarks> 
			/// This function assumes that there is at most one open connection at this time,
			/// which means that there can be no connections to other servers or clients.
			/// </remarks>
			RakNet::SystemAddress getServerAddress() const;			
			/// <summary>
            /// Handles the incoming packet
            /// </summary>
            /// <param name="a_Data">The data.</param>
            /// <param name="a_Event">The server event.</param>
            void handlePacket(RakNet::Packet* a_Data, unsigned char a_Event);
        };
    }
}