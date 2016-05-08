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

		m_RedScoredSource = std::make_unique<OpenALSource>("SFX/FlagSounds/red_scores.wav");
		m_FlagRedTakenSource = std::make_unique<OpenALSource>("stereo.wav");
		m_FlagRedReturnedSource = std::make_unique<OpenALSource>("SFX/FlagSounds/red_flag_returned.wav");

		m_BlueScoredSource = std::make_unique<OpenALSource>("SFX/FlagSounds/blue_flag_returned.wav");
		m_FlagBlueTakenSource = std::make_unique<OpenALSource>("SFX/FlagSounds/blue_flag_returned.wav");
		m_FlagBlueReturnedSource = std::make_unique<OpenALSource>("SFX/FlagSounds/blue_flag_returned.wav");
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
			m_BlueScoredSource->play();
		}
		else if (a_TeamIdentifier == ETeamIdentifier::TeamRed)
		{
			m_RedScoredSource->play();
		}
	}

	void Announcer::playFlagReturnedSound(ETeamIdentifier a_TeamIdentifier)
	{
		if (a_TeamIdentifier == ETeamIdentifier::TeamBlue)
		{
			m_FlagBlueReturnedSource->play();
		}
		else if (a_TeamIdentifier == ETeamIdentifier::TeamRed)
		{
			m_FlagRedReturnedSource->play();
		}
	}

	void Announcer::playFlagTakenSound(ETeamIdentifier a_TeamIdentifier)
	{
		if (a_TeamIdentifier == ETeamIdentifier::TeamBlue)
		{
			m_FlagBlueTakenSource->play();
		}
		else if (a_TeamIdentifier == ETeamIdentifier::TeamRed)
		{
			m_FlagRedTakenSource->play();
		}
	}


	Announcer::~Announcer()
	{
	}
}
