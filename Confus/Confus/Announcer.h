#pragma once
#include <memory>

#include "Flag.h"
#include "Player.h"

#include "OpenAL\OpenALSource.h"
namespace Confus
{
	class Announcer
	{
	private:
		Flag* m_RedFlag;
		Flag* m_BlueFlag;
		Player* m_Player;

		std::unique_ptr<OpenALSource> m_RedScoredSource;
		std::unique_ptr<OpenALSource> m_FlagRedTakenSource;
		std::unique_ptr<OpenALSource> m_FlagRedReturnedSource;

		std::unique_ptr<OpenALSource> m_BlueScoredSource;
		std::unique_ptr<OpenALSource> m_FlagBlueTakenSource;
		std::unique_ptr<OpenALSource> m_FlagBlueReturnedSource;
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


