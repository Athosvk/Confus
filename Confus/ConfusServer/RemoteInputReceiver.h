#pragma once
namespace ConfusShared
{
	class Player;
}

namespace RakNet
{
	class BitStream;
}

namespace ConfusServer
{
	namespace Networking
	{
		class Connection;
	}
	
	/// <summary> A controller that controls a player instance using the input from the remote client </summary>
	class RemoteInputReceiver
	{
	private:		
		/// <summary>The player this controller is controlling</summary>
		ConfusShared::Player& m_Player;		
		/// <summary>The connection through which it is being controlled</summary>
		Networking::Connection& m_Connection;

	public:		
		/// <summary>Initializes a new instance of the <see cref="RemoteController"/> class.</summary>
		/// <param name="a_Player">The player this controller is controlling</param>
		/// <param name="a_Connection">The connection through which it is being controlled</param>
		RemoteInputReceiver(ConfusShared::Player& a_Player, Networking::Connection& a_Connection);

	private:
		/// <summary>Synchronizes the player with the incoming data</summary>
		/// <param name="a_Data">The data to synchronize the instance with</param>
		void synchronize(RakNet::BitStream& a_Data) const;
	};
}
