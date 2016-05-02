#include <exception>

#include "AudioManager.h"

namespace Confus
{
	namespace Audio
	{
		AudioManager::AudioManager()
		{
			for(int i = 0; i < 4; ++i)
			{
				createNewAudioSource();
			}
		}

		void AudioManager::update()
		{
			for(auto& source : m_Sources)
			{
				source->updatePlayingState();
			}
		}

		OpenALSource* Confus::Audio::AudioManager::getAudioSource()
		{
			OpenALSource* freeSource = nullptr;
			for(auto& source : m_Sources)
			{
				if(!source->isPlaying())
				{
					freeSource = source.get();
				}
			}	
			if(freeSource == nullptr)
			{
				freeSource = createNewAudioSource();
			}
			return freeSource;
		}

		Sound AudioManager::createSound(const std::string& a_FilePath)
		{
			return Sound(a_FilePath, this);
		}

		OpenALSource* AudioManager::createNewAudioSource()
		{
			if(m_Sources.size() >= 16)
			{
				throw std::length_error("Cannot exceed hardware limit of 16 sources");
			}
			m_Sources.emplace_back(std::make_unique<OpenALSource>());
			return m_Sources.back().get();
		}
	}
}