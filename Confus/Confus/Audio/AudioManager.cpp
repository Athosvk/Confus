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
			return Sound(getBuffer(a_FilePath), this);
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

		OpenAL::OpenALBuffer* AudioManager::getBuffer(const std::string& a_FilePath)
		{
			OpenAL::OpenALBuffer* buffer = nullptr;
			auto iterator = m_BufferCache.find(a_FilePath);
			buffer = iterator != m_BufferCache.end() ? iterator->second.get() : createBuffer(a_FilePath);
			return buffer;
		}

		OpenAL::OpenALBuffer* AudioManager::createBuffer(const std::string& a_FilePath)
		{
			std::unique_ptr<OpenAL::OpenALBuffer> newBuffer = std::make_unique<OpenAL::OpenALBuffer>(a_FilePath);
			//Store this in advance; our local unique pointer will be empty/invalid once we move it into the 
			//the hashmap
			auto bufferHandle = newBuffer.get();
			m_BufferCache.emplace(a_FilePath, std::move(newBuffer));
			return bufferHandle;
		}
	}
}