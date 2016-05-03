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
			/// <summary> The position to play the sound at </summary>
			irr::core::vector3df m_Position = irr::core::vector3df();
			/// <summary> The velocity to play the sound with </summary>
			irr::core::vector3df m_Velocity = irr::core::vector3df();
			/// <summary> The direction to play the sound in  </summary>
			irr::core::matrix4 m_Direction = irr::core::IdentityMatrix;
			/// <summary> The speed to play the sound with </summary>
			/// <remarks> Equal to the pitch of the sound </remarks>
			float m_PlaySpeed = 1.0f;
			/// <summary> The volume to play the sound with </summary>
			float m_Volume = 1.0f;
			/// <summary> Whether to loop this sound </summary>
			bool m_Loop = false;
			/// <summary> The buffer to use for playing the sound </summary>
			OpenAL::OpenALBuffer* m_Buffer;
			/// <summary> The audio manager to use for retrieving an audio source </summary>
			AudioManager* m_AudioManager;
			/// <summary> The audio source to play this sound at </summary>
			OpenALSource* m_Source = nullptr;

		public:
			/// <summary> Initializes a new instance of the <see cref="Sound"/> class </summary>
			/// <param name="a_Buffer">The buffer to play </param>
			/// <param name="a_AudioManager">The audio manager to get audio sources from </param>
			Sound(OpenAL::OpenALBuffer* a_Buffer, AudioManager* a_AudioManager);
            
            /// <summary> Plays the sound </summary>
            void play();

            /// <summary> Stops the sound from playing </summary>
            void stop();

			/// <summary> Determines whether this sound is playing </summary>
			/// <returns> Whether this sound is being played </returns>
			bool isPlaying() const;

			/// <summary> Set the position of the sound </summary>
			/// <param name="a_Position">The new position</param>
			void setPosition(irr::core::vector3df a_Position);

			/// <summary> Set the velocity of the sound source, creating a doppler effect </summary>
			/// <param name="a_Velocity">The new velocity</param>
			void setVelocity(irr::core::vector3df a_Velocity);

			/// <summary> Set the direction of the sound source </summary>
			/// <param name="a_ForwardVector">The forward vector </param>
			/// <param name="a_UpVector">The up vector </param>
			void setDirection(irr::core::vector3df a_ForwardVector, irr::core::vector3df a_UpVector);

			/// <summary> Set the speed at which the source will be played </summary>
			void setPlaySpeed(float a_Speed);

			/// <summary> Set the volume of the source </summary>
			void setVolume(float a_Volume);

			/// <summary> Enables looping of this sound </summary>
			void enableLoop();

			/// <summary> Disables looping of this sound </summary>
			void disableLoop();
		};
	}
}