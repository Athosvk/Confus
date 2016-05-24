#pragma once
#include "../ConfusShared/Flag.h"
#include "../ConfusShared/Player.h"
#include "Audio\Sound.h"

namespace Confus
{		
	/// <summary>
	/// Plays sounds based on the events occuring in the GameWorld, such as the flag being taken
	/// </summary>
	class Announcer
	{
	private:		
		/// <summary>The red flag </summary>
		ConfusShared::Flag* m_RedFlag;		
		/// <summary>The blue flag </summary>
		ConfusShared::Flag* m_BlueFlag;		
		/// <summary>The player that we are announcing for</summary>
		ConfusShared::Player* m_Player;		
		/// <summary>The sound to play once the red team has scored a point</summary>
		Audio::Sound m_RedScoredSound;		
		/// <summary>The sound to play when the red team's flag has been taken</summary>
		Audio::Sound m_FlagRedTakenSound;		
		/// <summary>The sound to play once the flag of the red team has been returned</summary>
		Audio::Sound m_FlagRedReturnedSound;		
		/// <summary>The sound to play once the blue team has scored a point</summary>
		Audio::Sound m_BlueScoredSound;		
		/// <summary>The sound to play when the blue team's flag has been taken</summary>
		Audio::Sound m_FlagBlueTakenSound;		
		/// <summary>The sound to play once the flag of the blue team hs been returned</summary>
		Audio::Sound m_FlagBlueReturnedSound;

	private:		
		/// <summary>Plays the sound of the given team scoring a point by taking the eemy flag to their base</summary>
		/// <param name="a_TeamIdentifier">The team that scored the point</param>
		void playScoredSound(ConfusShared::ETeamIdentifier a_TeamIdentifier);		

		/// <summary>Plays the sound of the given team's flag being taken from their base</summary>
		/// <param name="a_TeamIdentifier">The team of which the flag was taken</param>
		void playFlagTakenSound(ConfusShared::ETeamIdentifier a_TeamIdentifier);
		
		/// <summary>Plays the sound of the flag being returned of the given team</summary>
		/// <param name="a_TeamIdentifier">The tema of which the flag was returned</param>
		void playFlagReturnedSound(ConfusShared::ETeamIdentifier a_TeamIdentifier);
	public:		
		/// <summary>Initializes a new instance of the <see cref="Announcer"/> class.</summary>
		/// <param name="a_RedFlag">The reference to the red team's flag</param>
		/// <param name="a_BlueFlag">The reference to the blue team's flag</param>
		/// <param name="a_Player">The reference to the local player instance</param>
		/// <param name="a_AudioManager">The audio manager that keeps track </param>
		Announcer(ConfusShared::Flag* a_RedFlag, ConfusShared::Flag* a_BlueFlag, ConfusShared::Player* a_Player, Audio::AudioManager* a_AudioManager);
		
		/// <summary>Plays a sound based on the flag event that occurred</summary>
		/// <param name="a_TeamIdentifier">The identifier for which team the flag event has occurred</param>
		/// <param name="a_PreviousFlagEnum">The previous flag state</param>
		/// <param name="a_CurrentFlagEnum">The current flag state</param>
		void playFlagEvent(ConfusShared::ETeamIdentifier a_TeamIdentifier, ConfusShared::EFlagEnum a_PreviousFlagEnum,
			ConfusShared::EFlagEnum a_CurrentFlagEnum);	

		/// <summary>Finalizes an instance of the <see cref="Announcer"/> class.</summary>
		~Announcer();
	};
}


