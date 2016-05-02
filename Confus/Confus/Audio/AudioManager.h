#pragma once
#include <vector>
#include <memory>

#include "../OpenAL/OpenALSource.h"
#include "Sound.h"

namespace Confus
{
	namespace Audio
	{
		class AudioManager
		{
		private:
			std::vector<std::unique_ptr<OpenALSource>> m_Sources;

		public:
			AudioManager();

			void update();
			OpenALSource* getAudioSource();
			Sound createSound(const std::string& a_FilePath);
		private:
			OpenALSource* createNewAudioSource();
		};
	}
}

