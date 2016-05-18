#include "Announcer.h"
#include "Audio\AudioManager.h"

namespace Confus
{
	Announcer::Announcer(ConfusShared::Flag* a_RedFlag, ConfusShared::Flag* a_BlueFlag, Player* a_Player, Audio::AudioManager* a_AudioManager)
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
		auto flagChangedEvents = [this](ConfusShared::ETeamIdentifier a_TeamIdentifier, ConfusShared::EFlagEnum a_PreviousFlagEnum, 
			ConfusShared::EFlagEnum a_CurrentFlagEnum) -> void 
		{ 
			playFlagEvent(a_TeamIdentifier, a_PreviousFlagEnum, a_CurrentFlagEnum); 
		};
		m_RedFlag->FlagStatusChangedEvent += flagChangedEvents;
		m_BlueFlag->FlagStatusChangedEvent += flagChangedEvents;

	}

	void Announcer::playFlagEvent(ConfusShared::ETeamIdentifier a_TeamIdentifier, ConfusShared::EFlagEnum a_PreviousFlagEnum, 
		ConfusShared::EFlagEnum a_CurrentFlagEnum)
	{
		if (a_PreviousFlagEnum == ConfusShared::EFlagEnum::FlagTaken && a_CurrentFlagEnum == ConfusShared::EFlagEnum::FlagBase)
		{
			playScoredSound(a_TeamIdentifier);
		}

		if (a_PreviousFlagEnum == ConfusShared::EFlagEnum::FlagDropped && a_CurrentFlagEnum == ConfusShared::EFlagEnum::FlagBase)
		{
			playFlagReturnedSound(a_TeamIdentifier);
		}

		if (a_PreviousFlagEnum != ConfusShared::EFlagEnum::FlagTaken && a_CurrentFlagEnum == ConfusShared::EFlagEnum::FlagTaken)
		{
			playFlagTakenSound(a_TeamIdentifier);
		}
	}

	void Announcer::playScoredSound(ConfusShared::ETeamIdentifier a_TeamIdentifier)
	{
		if (a_TeamIdentifier == ConfusShared::ETeamIdentifier::TeamBlue)
		{
			m_BlueScoredSound.play();
		}
		else if (a_TeamIdentifier == ConfusShared::ETeamIdentifier::TeamRed)
		{
			m_RedScoredSound.play();
		}
	}

	void Announcer::playFlagReturnedSound(ConfusShared::ETeamIdentifier a_TeamIdentifier)
	{
		if (a_TeamIdentifier == ConfusShared::ETeamIdentifier::TeamBlue)
		{
			m_FlagRedReturnedSound.play();
		}
		else if (a_TeamIdentifier == ConfusShared::ETeamIdentifier::TeamRed)
		{
			m_FlagBlueReturnedSound.play();
		}
	}

	void Announcer::playFlagTakenSound(ConfusShared::ETeamIdentifier a_TeamIdentifier)
	{
		if (a_TeamIdentifier == ConfusShared::ETeamIdentifier::TeamBlue)
		{
			m_FlagBlueTakenSound.play();
		}
		else if (a_TeamIdentifier == ConfusShared::ETeamIdentifier::TeamRed)
		{
			m_FlagRedTakenSound.play();
		}
	}

	Announcer::~Announcer()
	{
	}
}
