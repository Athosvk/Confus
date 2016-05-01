#pragma once
#include <memory>

#include "..\OpenAL\OpenALSource.h"

namespace Confus
{
	namespace Audio
	{
		class AudioLibrary
		{
		public:
			std::unique_ptr<OpenALSource> m_RedScoredSource;
			std::unique_ptr<OpenALSource> m_FlagRedTakenSource;
			std::unique_ptr<OpenALSource> m_FlagRedReturnedSource;

			std::unique_ptr<OpenALSource> m_BlueScoredSource;
			std::unique_ptr<OpenALSource> m_FlagBlueTakenSource;
			std::unique_ptr<OpenALSource> m_FlagBlueReturnedSource;
		public:
			AudioLibrary();
			~AudioLibrary();
		};
	}
}