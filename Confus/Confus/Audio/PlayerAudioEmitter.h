#pragma once
#include <irrlicht/irrlicht.h>
#include <memory>
#include <time.h> 

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
            OpenALSource* m_AudioSourceFootstep1;
            OpenALSource* m_AudioSourceFootstep2;
            OpenALSource* m_AudioSourceFootstep3;

            OpenALSource* m_AudioSourceGrunt1;
            OpenALSource* m_AudioSourceGrunt2;

            OpenALSource* m_AudioSourceSwordSwosh1;
            OpenALSource* m_AudioSourceSwordSwosh2;
            OpenALSource* m_AudioSourceSwordSwosh3;
            OpenALSource* m_AudioSourceSwordSwosh4;
            
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
            /// Plays a random attack sound.
            /// </summary>
            void playAttackSound();            
            /// <summary>
            /// Plays a random grunt.
            /// </summary>
            void playRandomGrunt();            
            /// <summary>
            /// Plays a random sword swosh.
            /// </summary>
            void playRandomSwordSwosh();
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

