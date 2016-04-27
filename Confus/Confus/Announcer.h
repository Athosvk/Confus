#pragma once
#include <memory>

#include "Flag.h"
#include "OpenAL\OpenALSource.h"
namespace Confus
{
	/// <summary> Enum that contains all the events that the announcer may react to </summary>
	enum class EAnnouncerEvent
	{
		None,
		Scored,
		FlagReturned,
		FlagTaken
	};

	class Announcer
	{
	private:
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
		Announcer();
		void PlayAnnouncerEvent(ETeamIdentifier a_TeamIdentifier, EAnnouncerEvent a_AnnouncerEvent);
		~Announcer();
	};
}

