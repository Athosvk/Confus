#include "Announcer.h"



namespace Confus
{

	Announcer::Announcer()
	{
		m_RedScoredSource = std::make_unique<OpenALSource>("test");
		m_FlagRedTakenSource = std::make_unique<OpenALSource>("test");
		m_FlagRedReturnedSource = std::make_unique<OpenALSource>("test");

		m_BlueScoredSource = std::make_unique<OpenALSource>("test");
		m_FlagBlueTakenSource = std::make_unique<OpenALSource>("test");
		m_FlagBlueReturnedSource = std::make_unique<OpenALSource>("test");
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
	}

	void Announcer::playFlagTakenSound(ETeamIdentifier a_TeamIdentifier)
	{
	}

	void Announcer::PlayAnnouncerEvent(ETeamIdentifier a_TeamIdentifier, EAnnouncerEvent a_AnnouncerEvent)
	{
		switch (a_AnnouncerEvent)
		{
		case EAnnouncerEvent::Scored: 
			playScoredSound(a_TeamIdentifier);
			break;
		case EAnnouncerEvent::FlagReturned:
			playFlagReturnedSound(a_TeamIdentifier);
			break;
		case EAnnouncerEvent::FlagTaken:
			playFlagTakenSound(a_TeamIdentifier);
			break;
		default: break;
		}
	}


	Announcer::~Announcer()
	{
	}
}
