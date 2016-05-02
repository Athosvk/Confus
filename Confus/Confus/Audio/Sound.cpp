#include "Sound.h"
#include "AudioManager.h"

namespace Confus
{
	namespace Audio
	{
		Sound::Sound(const std::string& a_FilePath, AudioManager* a_AudioManager)
			: m_AudioManager(a_AudioManager)
		{
			alGenBuffers(1, &m_Buffer);

			if(!OpenAL::ALFWLoadWaveToBuffer(OpenAL::ALFWaddMediaPath(a_FilePath.c_str()), m_Buffer))
			{
				throw std::invalid_argument("Path was invalid");
			}
		}

		Sound::~Sound()
		{
			//alDeleteBuffers(1, &m_Buffer);
		}

		void Sound::play()
		{
			m_Source = m_AudioManager->getAudioSource();
			m_Source->setPosition(m_Position);
			m_Source->setVelocity(m_Velocity);
			//Retrieve up and forward vectors from matrix rows
			m_Source->setDirection(m_Direction[0], m_Direction[1], m_Direction[2], m_Direction[4], m_Direction[5], m_Direction[6]);
			m_Source->setVolume(m_Volume);
			m_Source->setPlaySpeed(m_PlaySpeed);
			m_Loop ? m_Source->enableLoop() : m_Source->disableLoop();
			m_Source->play(m_Buffer, [this]
			{ 
				m_Source = nullptr;
			});
		}

        void Sound::stop()
        {
            if(!isPlaying())
            {
                throw std::logic_error("Sound is not being played");
            }
            m_Source->stop();
        }

        bool Sound::isPlaying() const
		{
			return m_Source != nullptr;
		}

		void Sound::setPosition(irr::core::vector3df a_Position)
		{
            if(m_Source != nullptr)
            {
                m_Source->setPosition(a_Position);
            }
			m_Position = a_Position;
		}

		void Sound::setVelocity(irr::core::vector3df a_Velocity)
		{
            if(m_Source != nullptr)
            {
                m_Source->setVelocity(a_Velocity);
            }
			m_Velocity = a_Velocity;
		}

		void Sound::setDirection(irr::core::vector3df a_ForwardVector, irr::core::vector3df a_UpVector)
		{
			m_Direction[0] = a_ForwardVector.X;
			m_Direction[1] = a_ForwardVector.Y;
			m_Direction[2] = a_ForwardVector.Z;

			m_Direction[4] = a_UpVector.X;
			m_Direction[5] = a_UpVector.Y;
			m_Direction[6] = a_UpVector.Z;
		}

		void Sound::setPlaySpeed(float a_Speed)
		{
			m_PlaySpeed = a_Speed;
		}

		void Sound::setVolume(float a_Volume)
		{
			m_Volume = a_Volume;
		}

		void Sound::enableLoop()
		{
			m_Loop = true;
		}

		void Sound::disableLoop()
		{
			m_Loop = false;
		}
	}
}
