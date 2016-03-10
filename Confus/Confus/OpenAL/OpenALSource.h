#pragma once
#include <Irrlicht\irrlicht.h>

#include "Framework/Framework.h"
#include <string>

namespace Confus {
	/// <summary>
	/// OpenAL Sound Source class.
	/// Needs the init() function to load a wave file.
	/// </summary>
	class OpenALSource {
    private:
        ALuint m_Buffer;
        ALuint m_Source;
        ALint m_PlayingState;
        std::string m_WaveFileString;
	private:
        /// <summary>
        /// Get rid of the buffer and source
        /// </summary>
		void dispose();
        /// <summary>
        /// Load the .wav wave file and set up the source + buffer.
        /// </summary>
        void init(std::string a_WaveFileString = "stereo.wav");
	public:
		/// <summary>
		/// Set the position of the sound source.
		/// </summary>
		void setPosition(float a_PositionX = 0.0f, float a_PositionY = 0.0f, float a_PositionZ = 0.0f);
        /// <summary>
        /// Set the position of the sound source.
        /// </summary>
        void setPosition(irr::core::vector3df a_Position);
		/// <summary>
		/// Set the velocity of the sound source.
		/// This creates a dooples effect.
		/// </summary>
		void setVelocity(float a_VelocityX = 0.0f, float a_VelocityY = 0.0f, float a_VelocityZ = 0.0f);
        /// <summary>
        /// Set the velocity of the sound source.
        /// This creates a doppler effect.
        /// </summary>
        void setVelocity(irr::core::vector3df a_Velocity);
		/// <summary>
		/// Set the direction of the sound source.
		/// At is the forward rotation vector at wich the object is facing and Up is the up rotation vector.
		/// </summary>
        void setDirection(irr::core::vector3df a_forwardVector, irr::core::vector3df a_upVector);
        /// <summary>
        /// Set the direction of the sound source.
        /// At is the forward rotation vector at wich the object is facing and Up is the up rotation vector.
        /// </summary>
		void setDirection(float a_AtX = 0.0f, float a_AtY = 0.0f, float a_AtZ = 0.0f, float a_UpX = 0.0f, float a_UpY = 0.0f, float a_UpZ = 0.0f);
		/// <summary>
		/// Set the speed at which the source will be played.
		/// </summary>
		void setPlaySpeed(float a_Speed = 1.0f);
		/// <summary>
		/// Set the volume of the source.
		/// </summary>
		void setVolume(float a_Volume);
		/// <summary>
		/// Set the pitch of the source.
		/// </summary>
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
		void play();
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
		OpenALSource(std::string a_WaveFileString);
		~OpenALSource();
	};
}