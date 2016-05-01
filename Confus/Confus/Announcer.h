#pragma once
#include <memory>

#include "Flag.h"
#include "Player.h"

#include "OpenAL\OpenALSource.h"
#include "Audio\AudioLibrary.h"
namespace Confus
{
	class Announcer
	{
	private:
		Flag* m_RedFlag;
		Flag* m_BlueFlag;
		Player* m_Player;
		Audio::AudioLibrary* m_lib;
	private:
		void playScoredSound(ETeamIdentifier a_TeamIdentifier);
		void playFlagTakenSound(ETeamIdentifier a_TeamIdentifier);
		void playFlagReturnedSound(ETeamIdentifier a_TeamIdentifier);
	public:
		Announcer(Flag* a_RedFlag, Flag* a_BlueFlag, Player* a_Player);
		void playFlagEvent(ETeamIdentifier a_TeamIdentifier, EFlagEnum a_PreviousFlagEnum, EFlagEnum a_CurrentFlagEnum);
		~Announcer();
	};
}


