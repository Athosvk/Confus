#pragma once
#include "framework/Win32/Framework.h"

class OpenALAudio {
public: 
	char* PlayASound();
private:
	static void LoopSound(ALuint sid, ALenum param, ALint value);
};