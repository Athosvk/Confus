#pragma once
#include "Framework/Framework.h"

namespace Confus
{
    namespace Audio
    {
        namespace OpenAL
        {

            class OpenALBuffer
            {
            private:
                ALuint m_ID = 0;

            public:
                OpenALBuffer(const std::string& a_FilePath);
                ~OpenALBuffer();

                ALuint getID() const;
            };
        }
    }
}
