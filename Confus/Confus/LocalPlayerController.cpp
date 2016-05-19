#include "LocalPlayerController.h"
#include "../ConfusShared/EventManager.h"

namespace Confus
{
	LocalPlayerController::LocalPlayerController(Player& a_Player, Networking::ClientConnection& a_Connection)
		: m_Player(a_Player),
		m_Connection(a_Connection)
	{
		m_Connection.addFunctionToMap(static_cast<unsigned char>(Networking::EPacketType::Player), [this](RakNet::Packet* a_Packet)
		{
			synchronizeState(RakNet::BitStream(a_Packet->data, a_Packet->length, false));
		});
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
		bitstream.Write(reinterpret_cast<char*>(&m_InputState));
		resetInputState();
	}

	void LocalPlayerController::resetInputState()
	{
		m_InputState.ForwardPressed = false;
		m_InputState.BackwardPressed = false;
		m_InputState.LeftPressed = false;
		m_InputState.RightPressed = false;
	}

	void LocalPlayerController::synchronizeState(RakNet::BitStream& a_Data)
	{
		irr::core::vector3df newPosition;
		a_Data.IgnoreBytes(sizeof(unsigned char));
		a_Data.Read(newPosition);
		m_Player.CameraNode->setPosition(newPosition);
	}
}