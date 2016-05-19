#include "RemotePlayerController.h"
#include "Networking/ClientConnection.h"
#include "../ConfusShared/Player.h"

namespace Confus
{
	RemotePlayerController::RemotePlayerController(Player& a_Player, Networking::ClientConnection& a_Connection)
		: m_Player(a_Player),
		m_Connection(a_Connection)
	{
		m_Connection.addFunctionToMap(static_cast<unsigned char>(Networking::EPacketType::Player), [this](RakNet::Packet* a_Packet)
		{
			synchronizeState(RakNet::BitStream(a_Packet->data, a_Packet->length, false));
		});
	}

	void RemotePlayerController::synchronizeState(RakNet::BitStream& a_Data)
	{
		irr::core::vector3df newPosition;
		a_Data.IgnoreBytes(sizeof(unsigned char));
		a_Data.Read(newPosition);
		m_Player.setPosition(newPosition);
	}
}