#include <RakNet/PacketPriority.h>

#include "LocalPlayerController.h"
#include "../ConfusShared/EventManager.h"
#include "../ConfusShared/Player.h"
#include "../ConfusShared/PacketType.h"

namespace Confus
{
	LocalPlayerController::LocalPlayerController(ConfusShared::Player& a_Player, Networking::ClientConnection& a_Connection)
		: m_Player(a_Player),
		m_Connection(a_Connection)
	{
	}

	void LocalPlayerController::handleInput(ConfusShared::EventManager& a_EventManager)
	{
		m_InputState.ForwardPressed = m_InputState.ForwardPressed || a_EventManager.IsKeyDown(irr::EKEY_CODE::KEY_KEY_W);
		m_InputState.BackwardPressed = m_InputState.BackwardPressed || a_EventManager.IsKeyDown(irr::EKEY_CODE::KEY_KEY_S);
		m_InputState.LeftPressed = m_InputState.LeftPressed || a_EventManager.IsKeyDown(irr::EKEY_CODE::KEY_KEY_A);
		m_InputState.RightPressed = m_InputState.RightPressed || a_EventManager.IsKeyDown(irr::EKEY_CODE::KEY_KEY_D);
		if(a_EventManager.IsRightMouseDown())
		{
			m_Player.startHeavyAttack();
		}
		else if(a_EventManager.IsLeftMouseDown())
		{
			m_Player.startLightAttack();
		};
	}

	void LocalPlayerController::fixedUpdate()
	{
		RakNet::BitStream bitstream;
		bitstream.Write(static_cast<RakNet::MessageID>(ConfusShared::Networking::EPacketType::Player));
		bitstream.Write(m_Player.getGUID());
		bitstream.Write(m_Player.getRotation());
		bitstream.Write(reinterpret_cast<char*>(&m_InputState));

		m_Connection.sendMessage(&bitstream, PacketReliability::UNRELIABLE);
		resetInputState();
	}

	void LocalPlayerController::resetInputState()
	{
		m_InputState.ForwardPressed = false;
		m_InputState.BackwardPressed = false;
		m_InputState.LeftPressed = false;
		m_InputState.RightPressed = false;
	}
}
