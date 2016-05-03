#include <stdexcept>

#include "OpenALBuffer.h"

namespace Confus
{
	namespace OpenAL
	{
		OpenALBuffer::OpenALBuffer(const std::string& a_FilePath)
		{
			alGenBuffers(1, &m_ID);
			if(!OpenAL::ALFWLoadWaveToBuffer(OpenAL::ALFWaddMediaPath(a_FilePath.c_str()), m_ID))
			{
				throw std::invalid_argument("Path was invalid");
			}
		}

		OpenALBuffer::~OpenALBuffer()
		{
			alDeleteBuffers(1, &m_ID);
		}

		ALuint OpenALBuffer::getID() const
		{
			return m_ID;
		}
	}
}