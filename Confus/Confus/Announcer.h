#pragma once
#include "../ConfusShared/Flag.h"
#include "Player.h"
#include "Audio\Sound.h"

namespace Confus
{
	class Announcer
	{
	private:
		Flag* m_RedFlag;
		Flag* m_BlueFlag;
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
		Announcer(Flag* a_RedFlag, Flag* a_BlueFlag, Player* a_Player, Audio::AudioManager* a_AudioManager);
		void playFlagEvent(ConfusShared::ETeamIdentifier a_TeamIdentifier, EFlagEnum a_PreviousFlagEnum, EFlagEnum a_CurrentFlagEnum);
		~Announcer();
	};
}


