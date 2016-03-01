#include "Flag.h"
#include "Player.h"
namespace Confus {

	Flag::Flag(irr::scene::ISceneManager* a_SceneManager, TeamIdentifier a_TeamIdentifier) {
		m_TeamIdentifier = a_TeamIdentifier;
	}

	//This class handles what to do on collision
	void Flag::CaptureFlag(TeamIdentifier a_PlayerTeamID, Player* a_PlayerObject) {
		if (m_FlagStatus == FLAG_TAKEN)
			return;

		if (a_PlayerTeamID != m_TeamIdentifier) {
			//Capture Flag
		}
		else if (a_PlayerTeamID == m_TeamIdentifier) {
			//If flag has been dropped return flag to base
			if (m_FlagStatus == FLAG_DROPPED) {
				Return();
			}
			//If flag is at base and player is carrying a flag
			else if (m_FlagStatus == FLAG_BASE) {
				if (a_PlayerObject->m_CarryingFlag == FLAG_TAKEN) {
					Score(a_PlayerObject);
				}
			}
		}
	}

	///Score points to team of a_PlayerObject
	void Flag::Score(Player* a_PlayerObject) {
		
	}

	///Drop Flag at position of player
	void Flag::Drop(Player* a_PlayerObject) {
		
	}

	Flag::~Flag() {

	}
}