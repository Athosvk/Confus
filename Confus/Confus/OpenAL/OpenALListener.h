#pragma once
#include <Irrlicht\irrlicht.h>

#include "Framework/Framework.h"


namespace Confus {
	class OpenALListener {
	public:
		/// <summary> 
        /// Set the position of the sound listener.
		/// </summary>
        /// <param name="a_PositionX">The position on the X axis.</param>
        /// <param name="a_PositionY">The position on the Y axis.</param>
        /// <param name="a_PositionZ">The position on the Z axis.</param>
		void setPosition(float a_PositionX = 0.0f, float a_PositionY = 0.0f, float a_PositionZ = 0.0f);
        void setPosition(irr::core::vector3df a_Position);
		/// <summary> Set the velocity of the sound listener.
		/// </summary>
		void setVelocity(float a_VelocityX = 0.0f, float a_VelocityY = 0.0f, float a_VelocityZ = 0.0f);
        void setVelocity(irr::core::vector3df a_VelocityX);
		/// <summary> Set the direction of the sound listener.
		/// At is the at rotation vector (forward) and Up is the up rotation vector.
		/// </summary>
		void setDirection(float a_AtX = 0.0f, float a_AtY = 0.0f, float a_AtZ = 0.0f, float a_UpX = 0.0f, float a_UpY = 0.0f, float a_UpZ = 0.0f);
        void setDirection(irr::core::vector3df a_ForwardVector, irr::core::vector3df a_UpVector);
        void dispose();
        void init();
	};
}