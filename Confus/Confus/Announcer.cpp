#include "Announcer.h"
#include "Audio\AudioManager.h"

namespace Confus
{

	Announcer::Announcer(Flag* a_RedFlag, Flag* a_BlueFlag, Player* a_Player, Audio::AudioManager* a_AudioManager)
		: m_RedFlag(a_RedFlag),
		m_BlueFlag(a_BlueFlag),
		m_Player(a_Player),
		m_RedScoredSound(a_AudioManager->createSound("SFX/FlagSounds/red_scores.wav")),
		m_FlagRedTakenSound(a_AudioManager->createSound("SFX/FlagSounds/red_flag_taken.wav")),
		m_FlagRedReturnedSound(a_AudioManager->createSound("SFX/FlagSounds/red_flag_returned.wav")),
		m_BlueScoredSound(a_AudioManager->createSound("SFX/FlagSounds/blue_scores.wav")),
		m_FlagBlueTakenSound(a_AudioManager->createSound("SFX/FlagSounds/blue_flag_taken.wav")),
		m_FlagBlueReturnedSound(a_AudioManager->createSound("SFX/FlagSounds/blue_flag_returned.wav"))
	{
		auto flagChangedEvents = [this](ETeamIdentifier a_TeamIdentifier, EFlagEnum a_PreviousFlagEnum, EFlagEnum a_CurrentFlagEnum) -> void 
		{ 
			playFlagEvent(a_TeamIdentifier, a_PreviousFlagEnum, a_CurrentFlagEnum); 
		};
		m_RedFlag->FlagStatusChangedEvent += flagChangedEvents;
		m_BlueFlag->FlagStatusChangedEvent += flagChangedEvents;

	}

	void Announcer::playFlagEvent(ETeamIdentifier a_TeamIdentifier, EFlagEnum a_PreviousFlagEnum, EFlagEnum a_CurrentFlagEnum)
	{
		if (a_PreviousFlagEnum == EFlagEnum::FlagTaken && a_CurrentFlagEnum == EFlagEnum::FlagBase)
		{
			playScoredSound(a_TeamIdentifier);
		}

		if (a_PreviousFlagEnum == EFlagEnum::FlagDropped && a_CurrentFlagEnum == EFlagEnum::FlagBase)
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
			m_BlueScoredSound.play();
		}
		else if (a_TeamIdentifier == ETeamIdentifier::TeamRed)
		{
			m_RedScoredSound.play();
		}
	}

	void Announcer::playFlagReturnedSound(ETeamIdentifier a_TeamIdentifier)
	{
		if (a_TeamIdentifier == ETeamIdentifier::TeamBlue)
		{
			m_FlagRedReturnedSound.play();
		}
		else if (a_TeamIdentifier == ETeamIdentifier::TeamRed)
		{
			m_FlagBlueReturnedSound.play();
		}
	}

	void Announcer::playFlagTakenSound(ETeamIdentifier a_TeamIdentifier)
	{
		if (a_TeamIdentifier == ETeamIdentifier::TeamBlue)
		{
			m_FlagBlueTakenSound.play();
		}
		else if (a_TeamIdentifier == ETeamIdentifier::TeamRed)
		{
			m_FlagRedTakenSound.play();
		}
	}

	Announcer::~Announcer()
	{
	}
}
