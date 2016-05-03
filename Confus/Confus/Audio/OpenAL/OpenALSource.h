#pragma once
#include <Irrlicht\irrlicht.h>
#include <functional>

#include "Framework/Framework.h"
#include <string>

namespace Confus 
{
	namespace OpenAL
	{
		class OpenALBuffer;
	}

    namespace Audio
    {
        /// <summary> Represntation of an OpenALSource/channel that can play various buffers </summary>
        /// <remarks> 
        /// This instance should be pooled, as there can only be as many instances of this class
        /// as the amount of channels supported by the driver, which is usually around 16
        /// </remarks>
        class OpenALSource
        {
        private:
            /// <summary> The source identifier for OpenAL </summary>
            ALuint m_Source = 0;
            /// <summary> The current playing state</summary>
            ALint m_PlayingState = 0;
            /// <summary> The function to call once the sound/buffer has finished playing </summary>
            std::function<void()> m_FinishedPlayingCallback = nullptr;

        public:
            /// <summary> Initializes a new instance of the <see cref="OpenALSource"/> class. </summary>
            OpenALSource();

            /// <summary> Finalizes an instance of the <see cref="OpenALSource"/> class. </summary>
            ~OpenALSource();
        private:
            /// <summary> Get rid of the buffer and source </summary>
            void dispose();

            /// <summary> Load the .wav wave file and set up the source + buffer. </summary>
            void init();

        public:
            /// <summary> Set the position at which sounds of this audiosoruce will be played from </summary>
            /// <param name="a_PositionX">The x position </param>
            /// <param name="a_PositionY">The y position </param>
            /// <param name="a_PositionZ">The z position </param>
            void setPosition(float a_PositionX, float a_PositionY, float a_PositionZ);

            /// <summary> Set the position at which sounds of this audiosoruce will be played from </summary>
            /// <param name="a_Position">The new position </param>
            void setPosition(irr::core::vector3df a_Position);

            /// <summary> Set the velocity at which sounds of this audiosoruce will be played at  </summary>
            /// <param name="a_VelocityX">The a_ velocity x.</param>
            /// <param name="a_VelocityY">The a_ velocity y.</param>
            /// <param name="a_VelocityZ">The a_ velocity z.</param>
            /// <remarks> This creates a doppler effect </remarks>
            void setVelocity(float a_VelocityX, float a_VelocityY, float a_VelocityZ);

            /// <summary> Set the velocity at which sounds of this audiosoruce will be played at </summary>
            /// <param name="a_Velocity">The new velocity </param>
            /// <remarks> This creates a doppler effect </remarks>
            void setVelocity(irr::core::vector3df a_Velocity);

            /// <summary> Set the direction in which sounds of this audiosoruce will be played in</summary>
            /// <param name="a_ForwardVector">The forward vector.</param>
            /// <param name="a_UpVector">The up vector.</param>
            void setDirection(irr::core::vector3df a_ForwardVector, irr::core::vector3df a_UpVector);

            /// <summary> Set the direction of the sound source </summary>
            /// <param name="a_AtX">The x of the forward vector </param>
            /// <param name="a_AtY">The y of the forward vector </param>
            /// <param name="a_AtZ">The z of the forward vector </param>
            /// <param name="a_UpX">The x of the up vector</param>
            /// <param name="a_UpY">The y of the up vector </param>
            /// <param name="a_UpZ">The z of the up vector </param>
            void setDirection(float a_AtX, float a_AtY, float a_AtZ, float a_UpX, float a_UpY, float a_UpZ);

            /// <summary> Sets the speed at which sounds/buffers of this audiosource will play at </summary>
            /// <param name="a_Speed">The playing speed </param>
            void setPlaySpeed(float a_Speed);

            /// <summary> Set the volume at which sounds/buffers of this audiosource will play wtih </summary>
            /// <param name="a_Volume">The new volume</param>
            void setVolume(float a_Volume);

            /// <summary> Enables looping of the sounds/buffers playing on this audio source </summary>
            void enableLoop();

            /// <summary> Disables looping of the sounds/buffers playing on this audio source </summary>
            void disableLoop();

            /// <summary> Start playing the sound if it's not </summary>
            /// <param name="a_Buffer">The buffer containing the sound to play</param>
            /// <param name="a_FinishedCallback">The callback to trigger once the sound has finished playing</param>
            void play(OpenAL::OpenALBuffer* a_Buffer, std::function<void()> a_FinishedCallback);

            /// <summary> Returns whether the sound is currently playing </summary>
            bool isPlaying();

            /// <summary>Pause the source </summary>
            void pause();

            /// <summary> Resumes the source from being paused </summary>
            void resume();

            /// <summary> Stops the audio source from playing </summary>
            void stop();

            /// <summary> Updates the playingstate, triggering the callback if the sound has finished </summary>
            void updatePlayingState();
        };
    }	
}