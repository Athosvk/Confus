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

	/// <summary>
	/// OpenAL Sound Source class.
	/// Needs the init() function to load a wave file.
	/// </summary>
	class OpenALSource 
	{
    private:
        ALuint m_Source = 0;
        ALint m_PlayingState = 0; 
        std::function<void()> m_FinishedPlayingCallback = nullptr;

	public:        
        /// <summary>
        /// Initializes a new instance of the <see cref="OpenALSource"/> class.
        /// </summary>
        OpenALSource();

        /// <summary>
        /// Finalizes an instance of the <see cref="OpenALSource"/> class.
        /// </summary>
        ~OpenALSource();
	private:
        /// <summary>
        /// Get rid of the buffer and source
        /// </summary>
		void dispose();

        /// <summary>
        /// Load the .wav wave file and set up the source + buffer.
        /// </summary>
        void init();

	public:
        /// <summary>
        /// Set the position of the sound source.
        /// </summary>
        /// <param name="a_PositionX">The a_ position x.</param>
        /// <param name="a_PositionY">The a_ position y.</param>
        /// <param name="a_PositionZ">The a_ position z.</param>
        void setPosition(float a_PositionX, float a_PositionY, float a_PositionZ);

        /// <summary>
        /// Set the position of the sound source.
        /// </summary>
        /// <param name="a_Position">The a_ position.</param>
        void setPosition(irr::core::vector3df a_Position);

        /// <summary>
        /// Set the velocity of the sound source.
        /// This creates a dooples effect.
        /// </summary>
        /// <param name="a_VelocityX">The a_ velocity x.</param>
        /// <param name="a_VelocityY">The a_ velocity y.</param>
        /// <param name="a_VelocityZ">The a_ velocity z.</param>
        void setVelocity(float a_VelocityX, float a_VelocityY, float a_VelocityZ);

        /// <summary>
        /// Set the velocity of the sound source.
        /// This creates a doppler effect.
        /// </summary>
        /// <param name="a_Velocity">The a_ velocity.</param>
        void setVelocity(irr::core::vector3df a_Velocity);

        /// <summary>
        /// Set the direction of the sound source.
        /// At is the forward rotation vector at wich the object is facing and Up is the up rotation vector.
        /// </summary>
        /// <param name="a_ForwardVector">The a_ forward vector.</param>
        /// <param name="a_UpVector">The a_ up vector.</param>
        void setDirection(irr::core::vector3df a_ForwardVector, irr::core::vector3df a_UpVector);

        /// <summary>
        /// Set the direction of the sound source.
        /// At is the forward rotation vector at wich the object is facing and Up is the up rotation vector.
        /// </summary>
		void setDirection(float a_AtX, float a_AtY, float a_AtZ, float a_UpX, float a_UpY, float a_UpZ);

        /// <summary>
        /// Set the speed at which the source will be played.
        /// </summary>
        /// <param name="a_Speed">The a_ speed.</param>
        void setPlaySpeed(float a_Speed);

        /// <summary>
        /// Set the volume of the source.
        /// </summary>
        /// <param name="a_Volume">The a_ volume.</param>
        void setVolume(float a_Volume);

        /// <summary>
        /// Set the pitch of the source.
        /// </summary>
        /// <param name="a_Pitch">The a_ pitch.</param>
        void setPitch(float a_Pitch);

		/// <summary>
		/// Enables Looping
		/// </summary>
		void enableLoop();

        /// <summary>
        /// Disables Looping
        /// </summary>
        void disableLoop();

        /// <summary>
        /// Start playing the sound if it's not
        /// </summary>
        /// <param name="a_Buffer">The buffer containing the sound to play</param>
        /// <param name="a_FinishedCallback">The callback to trigger once the sound has finished playing</param>
        void play(OpenAL::OpenALBuffer* a_Buffer, std::function<void()> a_FinishedCallback);

		/// <summary>
		/// Returns if the sound is currently playing
		/// </summary>
		bool isPlaying();

		/// <summary>
		/// Pause the source
		/// </summary>
		void pause();

		/// <summary>
		/// Resume the source from being paused.
		/// </summary>
		void resume();

		/// <summary>
		/// Stop playing
		/// </summary>
		void stop();     

        /// <summary>
        /// Updates the state of the playing.
        /// </summary>
        void updatePlayingState();
	};
}