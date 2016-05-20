#include <RakNet/BitStream.h>

#include "RemoteController.h"
#include "../ConfusShared/PacketType.h"
#include "Networking/Connection.h"
#include "../ConfusShared/Player.h"
#include "../ConfusShared/PlayerInputState.h"

namespace ConfusServer
{
	RemoteController::RemoteController(ConfusShared::Player& a_Player, Networking::Connection& a_Connection)
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
				synchronize(data);
			}
		});
	}

	void RemoteController::synchronize(RakNet::BitStream& a_Data) const
	{
		irr::core::vector3df rotation = irr::core::vector3df();
		a_Data.Read(rotation);
		m_Player.setRotation(rotation);

		ConfusShared::PlayerInputState inputState;
		a_Data.Read(inputState);
		irr::core::vector3df direction = irr::core::vector3df();
		if(inputState.ForwardPressed)
		{
			direction.Y = 1.0f;
		}
		else if(inputState.BackwardPressed)
		{
			direction.Y = -1.0f;
		}
		if(inputState.RightPressed)
		{
			direction.X = 1.0f;
		}
		else if(inputState.LeftPressed)
		{
			direction.X = -1.0f;
		}
		m_Player.setLocalDirection(direction.normalize());
	}
}
