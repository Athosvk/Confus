#include "RemoteInputReceiver.h"
#include "Networking/Connection.h"
#include "../ConfusShared/Player.h"
#include "../ConfusShared/Networking/PlayerStructs.h"
#include <RakNet/GetTime.h>

namespace ConfusServer
{
    const int RemoteInputReceiver::UserTimedOutTime = 20;

	RemoteInputReceiver::RemoteInputReceiver(ConfusShared::Player& a_Player, Networking::Connection& a_Connection)
		: m_Player(a_Player),
		m_Connection(a_Connection)
	{
        m_LastUpdateTime = RakNet::GetTimeMS() / 1000;
	}

    bool RemoteInputReceiver::userTimedOut() const
	{
        int currentTime = static_cast<int>((RakNet::GetTimeMS()) / 1000);
        return currentTime > m_LastUpdateTime + UserTimedOutTime;
	}

	void RemoteInputReceiver::synchronize(const ConfusShared::Networking::Client::PlayerUpdate& a_UpdateFromClient)
	{
        m_LastUpdateTime = RakNet::GetTimeMS() / 1000;
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
            m_Player.respawn();
		}
	}
}
