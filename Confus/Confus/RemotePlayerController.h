#pragma once
namespace RakNet
{
	class BitStream;
}

namespace Confus
{
	class Player;
	namespace Networking
	{
		class ClientConnection;
	}

	class RemotePlayerController
	{
	private:
		Player& m_Player;
		Networking::ClientConnection& m_Connection;
	public:
		RemotePlayerController(Player& a_Player, Networking::ClientConnection& a_Connection);

	private:
		void synchronizeState(RakNet::BitStream& a_Data);
	};
}