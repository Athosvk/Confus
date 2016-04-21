#pragma once
#include <RakNet/MessageIdentifiers.h>
#include <string>
#include <queue>

namespace RakNet
{
	class RakPeerInterface;
	struct SystemAddress;
}
namespace Confus
{
    namespace Networking
    {
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
		private:
			/// <summary> The type of packet </summary>
			enum class EPacketType : unsigned char
			{
				Message = 1 + ID_USER_PACKET_ENUM
			};
			/// <summary> The RakNet interface for interacting with RakNet </summary>
			RakNet::RakPeerInterface* m_Interface;
			/// <summary> The messages it was not able to send yet due to not having a connection established </summary>
			std::queue<std::string> m_StalledMessages;
			/// <summary> Whether we are connected to a server</summary>
			bool m_Connected = false;

        public:
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
			/// <param name="a_Message">The message contents</param>
			void sendMessage(const std::string& a_Message);
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
			/// Dispatches the messages that the connection was not able to send yet
			/// due to waiting for the connection to be established
			/// </summary>
			void dispatchStalledMessages();
        };
    }
}