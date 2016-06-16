#pragma once
namespace ConfusShared
{
    namespace Networking 
    {
        namespace Client 
        {
            struct PlayerUpdate;
        }
    }
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
        /// <summary>The time at which a player should time out in seconds</summary>
        static const int UserTimedOutTime;
		/// <summary>The player this controller is controlling</summary>
		ConfusShared::Player& m_Player;		
		/// <summary>The connection through which it is being controlled</summary>
		Networking::Connection& m_Connection;
        /// <summary>The time at which a player was last updated.</summary>
        int m_LastUpdateTime = 0;
	public:		
		/// <summary>Initializes a new instance of the <see cref="RemoteController"/> class.</summary>
		/// <param name="a_Player">The player this controller is controlling</param>
		/// <param name="a_Connection">The connection through which it is being controlled</param>
		RemoteInputReceiver(ConfusShared::Player& a_Player, Networking::Connection& a_Connection);

        /// <summary>Checks if a user timed out</summary>
        /// <returns>Returns if the player has timed out</returns>
        bool userTimedOut();

        /// <summary>Synchronizes the player with the incoming data</summary>
        /// <param name="a_Data">The data to synchronize the instance with</param>
        void synchronize(const ConfusShared::Networking::Client::PlayerUpdate& a_UpdateFromClient);
	};
}
