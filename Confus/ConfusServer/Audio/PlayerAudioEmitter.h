#pragma once
#include <irrlicht/irrlicht.h>
#include <memory>

#include "..\OpenAL\OpenALSource.h"

namespace ConfusServer
{
    namespace Audio
    {
        /// <summary>
        /// Class PlayerAudioEmitter emits sounds like footsteps and attacking sounds from each individual player.
        /// </summary>
        class PlayerAudioEmitter
        {
            OpenALSource* m_AudioSourceFootstep1;
            OpenALSource* m_AudioSourceFootstep2;
            OpenALSource* m_AudioSourceFootstep3;
            irr::scene::IAnimatedMeshSceneNode* m_AttachedPlayer;
        public:            
            /// <summary>
            /// Initializes a new instance of the <see cref="PlayerAudioEmitter"/> class.
            /// </summary>
            /// <param name="a_AttachedPlayer">The player that owns this emitter.</param>
            PlayerAudioEmitter(irr::scene::IAnimatedMeshSceneNode* a_AttachedPlayer);
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
            /// <summary>
            /// Creates the audio sources.
            /// </summary>
            void createAudioSources();
        };
    }
}

