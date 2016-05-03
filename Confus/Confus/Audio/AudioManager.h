#pragma once
#include <vector>
#include <memory>
#include <unordered_map>

#include "OpenAL/OpenALSource.h"
#include "OpenAL/OpenALBuffer.h"
#include "Sound.h"

namespace Confus
{
	namespace Audio
	{
		/// <summary>
		/// Keeps track of all the OpenALSource instances that used and pools them accordingly, so that they can easily be re-used
		/// by various sounds/buffers, while also making that wave files are only loaded into memory once and re-used otherwise
		/// </summary>
		class AudioManager
		{
		private:
			/// <summary> The audio source pool used for the playing of sounds </summary>
			/// <remarks> Individual sources allocated on the heap so that we can pass pointers to them around </remarks>
			std::vector<std::unique_ptr<OpenALSource>> m_Sources;
			/// <summary> The cache that keep strack of the used buffers by filename </summary>
			/// <remarks> Individual buffers allocated on the heap, so that we can pass pointers to them around </remarks>
			std::unordered_map<std::string, std::unique_ptr<OpenAL::OpenALBuffer>> m_BufferCache;

		public:
			/// <summary> Initializes a new instance of the <see cref="AudioManager"/> class </summary>
			AudioManager();
			
			/// <summary> Updates the audiosources so that they can provide callbacks to the sounds once done playing </summary>
			void update();

			/// <summary> Gets an inactive audiosource to play sounds with </summary>
			/// <returns> An inactive audiosource </returns>
			OpenALSource* getAudioSource();
			
			/// <summary>
			/// Creates a sound to play from
			/// </summary>
			/// <param name="a_FilePath">The path to the sound </param>
			/// <returns> The sound containing the either freshly loaded or cached buffer </returns>
			Sound createSound(const std::string& a_FilePath);
		private:
			/// <summary> Creates a new audio audio source in the pool </summary>
			/// <returns> The newly allocated audio source </returns>
			/// <remarks> Throws if the pool would exceed 16 audio sources, which is usually the hardware limit in terms of channels </remarks>
			OpenALSource* createNewAudioSource();

			/// <summary> Retrieves the buffer associated with a given file(path) </summary>
			/// <param name="a_FilePath">The filepath to the buffer/file</param>
			/// <returns> The new or cached buffer containing the audio data </returns>
			OpenAL::OpenALBuffer* getBuffer(const std::string& a_FilePath);

			/// <summary> Creates a buffer from the audio data at the given file(path) and places it into the cache </summary>
			/// <param name="a_FilePath">The file path </param>
			/// <returns> The newly allocated buffer </returns>
			OpenAL::OpenALBuffer* createBuffer(const std::string& a_FilePath);
		};
	}
}

