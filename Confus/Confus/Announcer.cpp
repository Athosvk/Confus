#include "Announcer.h"

namespace Confus
{

	Announcer::Announcer(Flag* a_RedFlag, Flag* a_BlueFlag, Player* a_Player)
		: m_RedFlag(a_RedFlag), m_BlueFlag(a_BlueFlag), m_Player(a_Player)
	{

		auto flagChangedEvents = [this](ETeamIdentifier a_TeamIdentifier, EFlagEnum a_PreviousFlagEnum, EFlagEnum a_CurrentFlagEnum) -> void 
		{ 
			playFlagEvent(a_TeamIdentifier, a_PreviousFlagEnum, a_CurrentFlagEnum); 
		};
		m_RedFlag->FlagStatusChangedEvent += flagChangedEvents;
		m_BlueFlag->FlagStatusChangedEvent += flagChangedEvents;
		m_lib = new Audio::AudioLibrary();
	}

	void Announcer::playFlagEvent(ETeamIdentifier a_TeamIdentifier, EFlagEnum a_PreviousFlagEnum, EFlagEnum a_CurrentFlagEnum)
	{
		if (a_PreviousFlagEnum == EFlagEnum::FlagTaken && a_CurrentFlagEnum == EFlagEnum::FlagBase)
		{
			playScoredSound(a_TeamIdentifier);
		}

		if (a_PreviousFlagEnum != EFlagEnum::FlagBase && a_CurrentFlagEnum == EFlagEnum::FlagBase)
		{
			playFlagReturnedSound(a_TeamIdentifier);
		}

		if (a_PreviousFlagEnum != EFlagEnum::FlagTaken && a_CurrentFlagEnum == EFlagEnum::FlagTaken)
		{
			playFlagTakenSound(a_TeamIdentifier);
		}
	}

	void Announcer::playScoredSound(ETeamIdentifier a_TeamIdentifier)
	{
		if (a_TeamIdentifier == ETeamIdentifier::TeamBlue)
		{
			m_lib->m_BlueScoredSource->play();
		}
		else if (a_TeamIdentifier == ETeamIdentifier::TeamRed)
		{
			m_lib->m_RedScoredSource->play();
		}
	}

	void Announcer::playFlagReturnedSound(ETeamIdentifier a_TeamIdentifier)
	{
		if (a_TeamIdentifier == ETeamIdentifier::TeamBlue)
		{
			m_lib->m_FlagBlueReturnedSource->play();
		}
		else if (a_TeamIdentifier == ETeamIdentifier::TeamRed)
		{
			m_lib->m_FlagRedReturnedSource->play();
		}
	}

	void Announcer::playFlagTakenSound(ETeamIdentifier a_TeamIdentifier)
	{
		if (a_TeamIdentifier == ETeamIdentifier::TeamBlue)
		{
			m_lib->m_FlagBlueTakenSource->play();
		}
		else if (a_TeamIdentifier == ETeamIdentifier::TeamRed)
		{
			//m_Player->m_SoundEmitter->m_FlagRedTakenSource->play();// m_FlagRedTakenSource->play();
			m_Player->m_SoundEmitter->test->m_FlagRedTakenSource->play();
		}
	}


	Announcer::~Announcer()
	{
		delete(m_lib);
	}
}
