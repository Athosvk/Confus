#include "RemotePlayerController.h"
#include "Networking/ClientConnection.h"
#include "../ConfusShared/Player.h"
#include "../ConfusShared/PacketType.h"

namespace Confus
{
	RemotePlayerController::RemotePlayerController(ConfusShared::Player& a_Player, Networking::ClientConnection& a_Connection)
		: m_Player(a_Player),
		m_Connection(a_Connection)
	{
		m_Connection.addFunctionToMap(static_cast<unsigned char>(ConfusShared::Networking::EPacketType::UpdatePosition), [this](RakNet::Packet* a_Packet)
		{
			RakNet::BitStream data(a_Packet->data, a_Packet->length, false);
			data.IgnoreBytes(sizeof(RakNet::MessageID));
			long long id;
			data.Read(id);
			if(id == m_Player.ID)
			{
				synchronizeState(data);
			}
		});
	}

	void RemotePlayerController::synchronizeState(RakNet::BitStream& a_Data) const
	{
		irr::core::vector3df newPosition;
		a_Data.Read(newPosition);
		m_Player.setPosition(newPosition);

		irr::core::vector3df newRotation;
		a_Data.Read(newRotation);
		m_Player.setRotation(newRotation);
	}
}
