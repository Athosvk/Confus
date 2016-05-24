#pragma once
#include "Framework/Framework.h"

namespace Confus
{
    namespace Audio
    {
        namespace OpenAL
        {			
			/// <summary>
			/// Represents a CPU side buffer to interface with OpenAL
			/// </summary>
			class OpenALBuffer
            {
            private:				
				/// <summary>The buffer ID</summary>
				ALuint m_ID = 0;

            public:				
				/// <summary>Initializes a new instance of the <see cref="OpenALBuffer"/> class.</summary>
				/// <param name="a_FilePath">The file path to the buffer to load</param>
				OpenALBuffer(const std::string& a_FilePath);
								
				/// <summary>Finalizes an instance of the <see cref="OpenALBuffer"/> class.</summary>
				~OpenALBuffer();
				
				/// <summary>Gets the identifier of the buffer</summary>
				/// <returns>The id of the buffer</returns>
				ALuint getID() const;
            };
        }
    }
}
