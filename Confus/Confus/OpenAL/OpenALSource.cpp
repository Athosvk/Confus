#include "OpenALSource.h"
namespace Confus {

	OpenALSource::OpenALSource(char* a_WaveFileString) {
		init(a_WaveFileString);
	}

	void OpenALSource::init(char* a_WaveFileString) {
		m_WaveFileString = a_WaveFileString;

		// Generate an AL Buffer
		alGenBuffers(1, &m_Buffer);

		// Load Wave file into OpenAL Buffer
		if (!OpenAL::ALFWLoadWaveToBuffer((char*)OpenAL::ALFWaddMediaPath(m_WaveFileString), m_Buffer))
		{
			return; //Something wrong with path
		}

		// Generate a Source to playback the Buffer
		alGenSources(1, &m_Source);

		// Attach Source to Buffer
		alSourcei(m_Source, AL_BUFFER, m_Buffer);
	}

	void OpenALSource::setPosition(float a_PositionX, float a_PositionY, float a_PositionZ) {
		alSource3f(m_Source, AL_POSITION, a_PositionX, a_PositionY, a_PositionZ);
	}

	void OpenALSource::setLoop(bool a_ShouldLoop) {
		alSourcei(m_Source, AL_LOOPING, a_ShouldLoop);
	}

	void OpenALSource::setVolume(float a_Volume) {
		alSourcei(m_Source, AL_GAIN, a_Volume);
	}

	void OpenALSource::setDirection(float a_AtX, float a_AtY, float a_AtZ, float a_UpX, float a_UpY, float a_UpZ) {
		ALfloat direction[6];
		//Forward Vector
		direction[0] = a_AtX;
		direction[1] = a_AtY;
		direction[2] = a_AtZ;

		//Up Vector
		direction[3] = a_UpX;
		direction[4] = a_UpY;
		direction[5] = a_UpZ;
		alSourcefv(m_Source, AL_DIRECTION, direction);
	}

	void OpenALSource::setVelocity(float a_VelocityX, float a_VelocityY, float a_VelocityZ) {
		alSource3f(m_Source, AL_VELOCITY, a_VelocityX, a_VelocityY, a_VelocityZ);
	}

	void OpenALSource::setPlaySpeed(float a_Speed) {
		alSourcef(m_Source, AL_PITCH, a_Speed);
	}

	void OpenALSource::pause() {
		alGetSourcei(m_Source, (ALenum)AL_SOURCE_STATE, &m_PlayingState);
		if (m_PlayingState == AL_PLAYING) {
			alSourcePause(m_Source);
		}
	}

	void OpenALSource::resume() {
		alGetSourcei(m_Source, (ALenum)AL_SOURCE_STATE, &m_PlayingState);
		if (m_PlayingState == AL_PAUSED) {
			alSourcePause(m_Source);
		}
		else if (m_PlayingState == AL_STOPPED || m_PlayingState == AL_INITIAL) {
			alSourcePlay(m_Source);
		}
	}

	void OpenALSource::stop() {
		alGetSourcei(m_Source, (ALenum)AL_SOURCE_STATE, &m_PlayingState);
		if (m_PlayingState == AL_PLAYING) {
			alSourceStop(m_Source);
		}
	}

	void OpenALSource::play() {
		if (isPlaying() == false) {
			//Play audio source
			alSourcePlay(m_Source);
		}
	}

	void OpenALSource::clear() {
		//Clean up by deleting Source(s) and Buffer(s)
		alSourceStop(m_Source);
		alDeleteSources(1, &m_Source);
		alDeleteBuffers(1, &m_Buffer);
		//ALFWShutdownOpenAL();
		//ALFWShutdown();
	}

	bool OpenALSource::isPlaying() {
		alGetSourcei(m_Source, (ALenum)AL_SOURCE_STATE, &m_PlayingState);
		return (m_PlayingState == AL_PLAYING);
	}

	OpenALSource::~OpenALSource() {
		clear();
	}
}