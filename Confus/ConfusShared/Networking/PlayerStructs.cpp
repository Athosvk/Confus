#include "PlayerStructs.h"

namespace ConfusShared
{
	namespace Networking
	{
		// Origin of these packets is from the server
		namespace Server
		{
			NewPlayer::NewPlayer(ConfusShared::Player* a_Player)
				: ID(a_Player->getGUID()),
				Team(a_Player->getTeamIdentifier()),
				Position(a_Player->getPosition()),
				Rotation(a_Player->getRotation())
			{
			}

            PlayerUpdate::PlayerUpdate(ConfusShared::Player * a_Player)
                : ID(a_Player->getGUID()),
                Position(a_Player->getPosition()),
                Rotation(a_Player->getRotation()),
                State(a_Player->getState())
            {
            }
        }
	}
}