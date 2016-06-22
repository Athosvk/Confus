#include "Announcer.h"
#include "Audio/AudioManager.h"

namespace Confus
{
	Announcer::Announcer(ConfusShared::Flag* a_RedFlag, ConfusShared::Flag* a_BlueFlag, ConfusShared::Player* a_Player, Audio::AudioManager* a_AudioManager)
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
		m_RedFlag->addScoreCallback([this]()
		{
			playScoredSound(ConfusShared::ETeamIdentifier::TeamBlue);
		});
		m_BlueFlag->addScoreCallback([this]()
		{
			playScoredSound(ConfusShared::ETeamIdentifier::TeamRed);
		});
	}

	void Announcer::update()
	{
		m_RedScoredSound.setPosition(m_Player->getAbsolutePosition());
		m_FlagRedTakenSound.setPosition(m_Player->getAbsolutePosition());
		m_FlagRedReturnedSound.setPosition(m_Player->getAbsolutePosition());
		m_BlueScoredSound.setPosition(m_Player->getAbsolutePosition());
		m_FlagBlueTakenSound.setPosition(m_Player->getAbsolutePosition());
		m_FlagBlueReturnedSound.setPosition(m_Player->getAbsolutePosition());
	}

	void Announcer::playFlagEvent(ConfusShared::ETeamIdentifier a_TeamIdentifier, ConfusShared::EFlagEnum a_PreviousFlagEnum, 
		ConfusShared::EFlagEnum a_CurrentFlagEnum)
	{
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
			m_FlagBlueReturnedSound.play();
		}
		else if (a_TeamIdentifier == ConfusShared::ETeamIdentifier::TeamRed)
		{
			m_FlagRedReturnedSound.play();
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
}
