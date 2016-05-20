#pragma once
namespace RakNet
{
	class BitStream;
}

namespace ConfusShared
{
	class Player;
}

namespace Confus
{
	namespace Networking
	{
		class ClientConnection;
	}

	class RemotePlayerController
	{
	private:
		ConfusShared::Player& m_Player;
		Networking::ClientConnection& m_Connection;
	public:
		RemotePlayerController(ConfusShared::Player& a_Player, Networking::ClientConnection& a_Connection);

	private:
		void synchronizeState(RakNet::BitStream& a_Data) const;
	};
}