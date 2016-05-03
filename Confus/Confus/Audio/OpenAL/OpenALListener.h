#pragma once
#include <Irrlicht\irrlicht.h>

#include "Framework/Framework.h"

namespace Confus 
{
    namespace Audio
    {        
        /// <summary> OpenALListener is used to listen to all the OpenAL sounds.
        /// <remarks> There can only be one listener and it's managed by this class. </remarks>
        /// </summary>
        class OpenALListener {
        public:
            /// <summary>
            /// Initializes a new instance of the <see cref="OpenALListener"/> class.
            /// </summary>
            OpenALListener();

            /// <summary> Sets the position of the sound listener. </summary>
            /// <param name="a_PositionX">The position on the x axis.</param>
            /// <param name="a_PositionY">The position on the y axis.</param>
            /// <param name="a_PositionZ">The position on the z axis.</param>
            void setPosition(float a_PositionX = 0.0f, float a_PositionY = 0.0f, float a_PositionZ = 0.0f);

            /// <summary> Sets the position of the sound listener.  </summary>
            /// <param name="a_Position">The a_ position.</param>
            void setPosition(irr::core::vector3df a_Position);
                       
            /// <summary> Sets the velocity of the sound listener. </summary>
            /// <param name="a_VelocityX">The velocity on the x axis.</param>
            /// <param name="a_VelocityY">The velocity on the y axis .</param>
            /// <param name="a_VelocityZ">The velocity on the z axis.</param>
            void setVelocity(float a_VelocityX = 0.0f, float a_VelocityY = 0.0f, float a_VelocityZ = 0.0f);            

            /// <summary> Sets the velocity of the sound listener </summary>
            /// <param name="a_Velocity">The velocity of the sound listener.</param>
            void setVelocity(irr::core::vector3df a_Velocity);

            /// <summary> Set the direction of the sound listener. At being the forward rotation vector and Up the up rotation vector of the listener. </summary>
            void setDirection(float a_AtX = 0.0f, float a_AtY = 0.0f, float a_AtZ = 0.0f, float a_UpX = 0.0f, float a_UpY = 0.0f, float a_UpZ = 0.0f);          

            /// <summary> Sets the direction of the sound listener. </summary>
            /// <param name="a_ForwardVector">The forward vector of the sound listener.</param>
            /// <param name="a_UpVector">The up vector of the sound listener.</param>
            void setDirection(irr::core::vector3df a_ForwardVector, irr::core::vector3df a_UpVector); 

            /// <summary>
            /// Disposes the sound listener.
            /// </summary>
            void dispose();

            /// <summary>
            /// Initializes this instance.
            /// </summary>
            void init();
        };
    }
}