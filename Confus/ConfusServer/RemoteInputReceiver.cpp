#include <RakNet/BitStream.h>

#include "RemoteInputReceiver.h"
#include "../ConfusShared/PacketType.h"
#include "Networking/Connection.h"
#include "../ConfusShared/Player.h"
#include "../ConfusShared/Networking/PlayerStructs.h"

namespace ConfusServer
{
	RemoteInputReceiver::RemoteInputReceiver(ConfusShared::Player& a_Player, Networking::Connection& a_Connection)
		: m_Player(a_Player),
		m_Connection(a_Connection)
	{
		m_Connection.addFunctionToMap(static_cast<unsigned char>(ConfusShared::Networking::EPacketType::Player), [this](RakNet::Packet* a_Packet)
		{
			RakNet::BitStream data(a_Packet->data, a_Packet->length, false);
			data.IgnoreBytes(sizeof(RakNet::MessageID));

			ConfusShared::Networking::Client::PlayerUpdate updateFromClient;
			data.Read(updateFromClient);
			if(updateFromClient.ID == m_Player.getGUID())
			{
				synchronize(updateFromClient);
			}
		});
	}

	void RemoteInputReceiver::synchronize(const ConfusShared::Networking::Client::PlayerUpdate& a_UpdateFromClient) const
	{
		m_Player.setRotation(a_UpdateFromClient.Rotation);
		irr::core::vector3df direction = irr::core::vector3df();
		if(a_UpdateFromClient.InputState.ForwardPressed)
		{
			direction.Z = 1.0f;
		}
		else if(a_UpdateFromClient.InputState.BackwardPressed)
		{
			direction.Z = -1.0f;
		}
		if(a_UpdateFromClient.InputState.RightPressed)
		{
			direction.X = 1.0f;
		}
		else if(a_UpdateFromClient.InputState.LeftPressed)
		{
			direction.X = -1.0f;
		}
		if(direction.getLengthSQ() > 0.0f)
		{
			direction = direction.normalize();
		}
		m_Player.setWalkingDirection(direction);

		if (a_UpdateFromClient.InputState.LeftMouseButtonPressed)
		{
			m_Player.changeState(ConfusShared::EPlayerState::LightAttacking);
		} 
		else if (a_UpdateFromClient.InputState.RightMouseButtonPressed)
		{
			m_Player.changeState(ConfusShared::EPlayerState::HeavyAttacking);
		}
	}
}
