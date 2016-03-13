#pragma once
#include <irrlicht/irrlicht.h>
#include <memory>

#include "..\Player.h"
#include "..\OpenAL\OpenALSource.h"

namespace Confus
{
    namespace Audio
    {
        /// <summary>
        /// Class PlayerAudioEmitter emits sounds like footsteps and attacking sounds from each individual player.
        /// </summary>
        class PlayerAudioEmitter
        {
            OpenALSource* m_AudioSource;
            Player* m_AttachedPlayer;
        public:            
            /// <summary>
            /// Initializes a new instance of the <see cref="PlayerAudioEmitter"/> class.
            /// </summary>
            /// <param name="a_AttachedPlayer">The player that owns this emitter.</param>
            PlayerAudioEmitter(Player a_AttachedPlayer);
            /// <summary>
            /// Finalizes an instance of the <see cref="PlayerAudioEmitter"/> class.
            /// </summary>
            ~PlayerAudioEmitter();

            /// <summary>
            /// Plays a random footstep sound.
            /// </summary>
            void playFootStepSound();
            
            /// <summary>
            /// Updates the position of this instance.
            /// </summary>
            void updatePosition();

        };
    }
}

