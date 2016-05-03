#pragma once
#include <Irrlicht/irrlicht.h>
#include <string>

namespace Confus
{
	namespace OpenAL
	{
		class OpenALBuffer;
	}
    class OpenALSource;

	namespace Audio
	{
		class AudioManager;

		class Sound
		{
		private:
			irr::core::vector3df m_Position;
			irr::core::vector3df m_Velocity;
			irr::core::matrix4 m_Direction = irr::core::IdentityMatrix;
			float m_PlaySpeed = 1.0f;
			float m_Volume = 1.0f;
			bool m_Loop = false;
			OpenAL::OpenALBuffer* m_Buffer;
			AudioManager* m_AudioManager;
			OpenALSource* m_Source = nullptr;

		public:
			Sound(OpenAL::OpenALBuffer* a_Buffer, AudioManager* a_AudioManager);
            
            /// <summary>
            /// Plays this instance.
            /// </summary>
            void play();
            /// <summary>
            /// Stops this instance.
            /// </summary>
            void stop();
			bool isPlaying() const;

			/// <summary>
			/// Set the position of the sound source.
			/// </summary>
			void setPosition(irr::core::vector3df a_Position);

			/// <summary>
			/// Set the velocity of the sound source.
			/// This creates a doppler effect.
			/// </summary>
			void setVelocity(irr::core::vector3df a_Velocity);

			/// <summary>
			/// Set the direction of the sound source.
			/// At is the forward rotation vector at wich the object is facing and Up is the up rotation vector.
			/// </summary>
			void setDirection(irr::core::vector3df a_ForwardVector, irr::core::vector3df a_UpVector);

			/// <summary>
			/// Set the speed at which the source will be played.
			/// </summary>
			void setPlaySpeed(float a_Speed);

			/// <summary>
			/// Set the volume of the source.
			/// </summary>
			void setVolume(float a_Volume);

			/// <summary>
			/// Enables Looping
			/// </summary>
			void enableLoop();

			/// <summary>
			/// Disables Looping
			/// </summary>
			void disableLoop();
		};
	}
}