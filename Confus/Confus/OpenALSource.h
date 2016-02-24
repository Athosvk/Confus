#pragma once
#include "framework/Win32/Framework.h"

/// <summary>
/// OpenAL Sound Source class.
/// Needs the init() function to load a wave file.
/// </summary>
class OpenALSource {
private:
	void clear();
public: 
	/// <summary>
	/// Load the .wav wave file and set up the source + buffer.
	/// </summary>
	void init(char* a_WaveFileString = "stereo.wav");
	/// <summary>
	/// Set the direction of the sound source.
	/// At is the at rotation vector (forward) and Up is the up rotation vector.
	/// </summary>
	void setPosition(float a_PositionX = 0.0f, float a_PositionY = 0.0f, float a_PositionZ = 0.0f);
	/// <summary>
	/// Set the velocity of the sound source.
	/// This creates a dooples effect.
	/// </summary>
	void setVelocity(float a_VelocityX = 0.0f, float a_VelocityY = 0.0f, float a_VelocityZ = 0.0f);
	/// <summary>
	/// Set the direction of the sound source.
	/// At is the at rotation vector (forward) and Up is the up rotation vector.
	/// </summary>
	void setDirection(float a_AtX = 0.0f, float a_AtY = 0.0f, float a_AtZ = 0.0f, float a_UpX = 0.0f, float a_UpY = 0.0f, float a_UpZ = 0.0f);
	/// <summary>
	/// Set the speed at which the file will be played.
	/// </summary>
	void setPlaySpeed(float a_Speed = 1.0f);
	/// <summary>
	/// Set the pitch of the wave.
	/// </summary>
	void setPitch(float a_Pitch);
	void setWavePath(char* a_WavePath);
	void setLoop(bool a_ShouldLoop = false);
	void play();
	bool isPlaying();
	void pause();
	void resume();
	void stop();
	~OpenALSource();
private:
	ALuint      m_Buffer;
	ALuint      m_Source;
	ALint       m_PlayingState;
	char* m_WaveFileString;
};