#pragma once
#include <vector>
#include <memory>
#include <unordered_map>

#include "../OpenAL/OpenALSource.h"
#include "../OpenAL/OpenALBuffer.h"
#include "Sound.h"

namespace Confus
{
	namespace Audio
	{
		class AudioManager
		{
		private:
			std::vector<std::unique_ptr<OpenALSource>> m_Sources;
			std::unordered_map<std::string, std::unique_ptr<OpenAL::OpenALBuffer>> m_BufferCache;

		public:
			AudioManager();

			void update();
			OpenALSource* getAudioSource();
			Sound createSound(const std::string& a_FilePath);
		private:
			OpenALSource* createNewAudioSource();
			OpenAL::OpenALBuffer* getBuffer(const std::string& a_FilePath);
			OpenAL::OpenALBuffer* createBuffer(const std::string& a_FilePath);
		};
	}
}

