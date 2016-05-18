#pragma once
#include "../ConfusShared/Flag.h"
#include "Player.h"
#include "Audio\Sound.h"

namespace Confus
{
	class Announcer
	{
	private:
		ConfusShared::Flag* m_RedFlag;
		ConfusShared::Flag* m_BlueFlag;
		Player* m_Player;

		Audio::Sound m_RedScoredSound;
		Audio::Sound m_FlagRedTakenSound;
		Audio::Sound m_FlagRedReturnedSound;

		Audio::Sound m_BlueScoredSound;
		Audio::Sound m_FlagBlueTakenSound;
		Audio::Sound m_FlagBlueReturnedSound;

	private:
		void playScoredSound(ConfusShared::ETeamIdentifier a_TeamIdentifier);
		void playFlagTakenSound(ConfusShared::ETeamIdentifier a_TeamIdentifier);
		void playFlagReturnedSound(ConfusShared::ETeamIdentifier a_TeamIdentifier);
	public:
		Announcer(ConfusShared::Flag* a_RedFlag, ConfusShared::Flag* a_BlueFlag, Player* a_Player, Audio::AudioManager* a_AudioManager);
		void playFlagEvent(ConfusShared::ETeamIdentifier a_TeamIdentifier, ConfusShared::EFlagEnum a_PreviousFlagEnum, 
			ConfusShared::EFlagEnum a_CurrentFlagEnum);
		~Announcer();
	};
}


