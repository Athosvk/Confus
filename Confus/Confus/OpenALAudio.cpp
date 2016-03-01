/*
* Copyright (c) 2006, Creative Labs Inc.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted provided
* that the following conditions are met:
*
*     * Redistributions of source code must retain the above copyright notice, this list of conditions and
* 	     the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions
* 	     and the following disclaimer in the documentation and/or other materials provided with the distribution.
*     * Neither the name of Creative Labs Inc. nor the names of its contributors may be used to endorse or
* 	     promote products derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/

#include "OpenALAudio.h"
#include <iostream>
#include <thread>         // std::thread

#define	TEST_WAVE_FILE		"stereo.wav"


char* OpenALAudio::PlayASound()
{
	ALuint      uiBuffer;
	ALuint      uiSource;
	ALint       iState;

	// Initialize Framework
	ALFWInit();

	std::cout << "PlayStatic Test Application\n";

	if (!ALFWInitOpenAL())
	{
		ALFWShutdown();
		//return -1;
		return "Failed to initialize OpenAL\n";
	}

	// Generate an AL Buffer
	alGenBuffers(1, &uiBuffer);

	// Load Wave file into OpenAL Buffer
	if (!ALFWLoadWaveToBuffer((char*)ALFWaddMediaPath(TEST_WAVE_FILE), uiBuffer))
	{
		//return -2;
		return ALFWaddMediaPath(TEST_WAVE_FILE);
	}

	// Generate a Source to playback the Buffer
	alGenSources(1, &uiSource);

	// Attach Source to Buffer
	alSourcei(uiSource, AL_BUFFER, uiBuffer);

	// Play Source
	alSourcePlay(uiSource);
	std::cout << "Playing Source ";

	//Sound thread
	std::thread soundThread(OpenALAudio::LoopSound, uiSource, (ALenum)AL_SOURCE_STATE, iState);
	soundThread.join();

	// Clean up by deleting Source(s) and Buffer(s)
	alSourceStop(uiSource);
	alDeleteSources(1, &uiSource);
	alDeleteBuffers(1, &uiBuffer);
	ALFWShutdownOpenAL();
	ALFWShutdown();

	//return 0;
	return "Played Sound!";
}

void OpenALAudio::LoopSound(ALuint sid, ALenum param, ALint value) {
	do
	{
		Sleep(100);
		std::cout << ".";
		// Get Source State
		alGetSourcei(sid, param, &value);
	} while (value == AL_PLAYING);
}