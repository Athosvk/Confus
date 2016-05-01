#include <time.h> 

#include "AudioLibrary.h"

namespace Confus
{
	namespace Audio
	{
		AudioLibrary::AudioLibrary()
		{
			m_RedScoredSource = std::make_unique<OpenALSource>("SFX/FlagSounds/red_scores.wav");
			m_FlagRedTakenSource = std::make_unique<OpenALSource>("stereo.wav");
			m_FlagRedReturnedSource = std::make_unique<OpenALSource>("SFX/FlagSounds/red_flag_returned.wav");

			m_BlueScoredSource = std::make_unique<OpenALSource>("SFX/FlagSounds/blue_flag_returned.wav");
			m_FlagBlueTakenSource = std::make_unique<OpenALSource>("SFX/FlagSounds/blue_flag_returned.wav");
			m_FlagBlueReturnedSource = std::make_unique<OpenALSource>("SFX/FlagSounds/blue_flag_returned.wav");
		}
		AudioLibrary::~AudioLibrary()
		{

		}
	}
}