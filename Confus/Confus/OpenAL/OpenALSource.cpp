#include <stdexcept>
#include <string>

#include "OpenALSource.h"

namespace Confus {

    OpenALSource::OpenALSource(std::string a_WaveFileString) : m_WaveFileString(a_WaveFileString) {
        init(a_WaveFileString);
    }

    OpenALSource::~OpenALSource() {
        dispose();
    }

    void OpenALSource::init(std::string a_WaveFileString) {
        m_WaveFileString = a_WaveFileString;
        m_Buffer = new ALuint;
        m_Source = new ALuint;
        m_PlayingState = new ALint;
        alGenBuffers(1, m_Buffer);

        if(!OpenAL::ALFWLoadWaveToBuffer((char*)OpenAL::ALFWaddMediaPath(m_WaveFileString.c_str()), *m_Buffer))
        {
            throw std::invalid_argument("Path was invalid");
        }

        alGenSources(1, m_Source);
        alSourcei(*m_Source, AL_BUFFER, *m_Buffer);
    }

    void OpenALSource::setPosition(float a_PositionX, float a_PositionY, float a_PositionZ) {
        alSource3f(*m_Source, AL_POSITION, a_PositionX, a_PositionY, a_PositionZ);
    }

    void OpenALSource::setPosition(irr::core::vector3df a_Position) {
        alSource3f(*m_Source, AL_POSITION, a_Position.X, a_Position.Y, a_Position.Z);
    }

    void OpenALSource::enableLoop() {
        alSourcef(*m_Source, AL_LOOPING, AL_TRUE);
    }

    void OpenALSource::disableLoop() {
        alSourcef(*m_Source, AL_LOOPING, AL_FALSE);
    }

    void OpenALSource::setVolume(float a_Volume) {
        alSourcef(*m_Source, AL_GAIN, a_Volume);
    }

    void OpenALSource::setPitch(float a_Pitch) {
        alSourcef(*m_Source, AL_PITCH, a_Pitch);
    }

    void OpenALSource::setDirection(irr::core::vector3df a_forwardVector, irr::core::vector3df a_UpVector) {
        setDirection(a_forwardVector.X, a_forwardVector.Y, a_forwardVector.Z, a_UpVector.X, a_UpVector.Y, a_UpVector.Z);
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
        alSourcefv(*m_Source, AL_DIRECTION, direction);
    }

    void OpenALSource::setVelocity(float a_VelocityX, float a_VelocityY, float a_VelocityZ) {
        alSource3f(*m_Source, AL_VELOCITY, a_VelocityX, a_VelocityY, a_VelocityZ);
    }

    void OpenALSource::setVelocity(irr::core::vector3df a_Velocity) {
        alSource3f(*m_Source, AL_VELOCITY, a_Velocity.X, a_Velocity.Y, a_Velocity.Z);
    }

    void OpenALSource::setPlaySpeed(float a_Speed) {
        alSourcef(*m_Source, AL_PITCH, a_Speed);
    }

    void OpenALSource::pause() {
        alGetSourcei(*m_Source, static_cast<ALenum>(AL_SOURCE_STATE), m_PlayingState);
        if(*m_PlayingState == AL_PLAYING) {
            alSourcePause(*m_Source);
        }
    }

    void OpenALSource::resume() {
        alGetSourcei(*m_Source, static_cast<ALenum>(AL_SOURCE_STATE), m_PlayingState);
        if(*m_PlayingState == AL_PAUSED) {
            alSourcePause(*m_Source);
        }
        else if(*m_PlayingState == AL_STOPPED || *m_PlayingState == AL_INITIAL) {
            alSourcePlay(*m_Source);
        }
    }

    void OpenALSource::stop() {
        alGetSourcei(*m_Source, static_cast<ALenum>(AL_SOURCE_STATE), m_PlayingState);
        if(*m_PlayingState == AL_PLAYING) {
            alSourceStop(*m_Source);
        }
    }

    void OpenALSource::play() {
        if(isPlaying() == false) {
            alSourcePlay(*m_Source);
        }
    }

    void OpenALSource::dispose() {
        alSourceStop(*m_Source);
        alDeleteSources(1, m_Source);
        alDeleteBuffers(1, m_Buffer);
        delete(m_Source);
        delete(m_Buffer);
        delete(m_PlayingState);
    }

    bool OpenALSource::isPlaying() {
        alGetSourcei(*m_Source, (ALenum)AL_SOURCE_STATE, m_PlayingState);
        return (*m_PlayingState == AL_PLAYING);
    }
}