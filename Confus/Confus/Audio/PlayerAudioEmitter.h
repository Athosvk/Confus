#pragma once
#include <irrlicht/irrlicht.h>
#include <vector>
#include <memory>

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
            OpenALSource* m_AudioSourceFootsteps[3];
            OpenALSource* m_AudioSourceGrunts[3];
            OpenALSource* m_AudioSourceSwordSwoshes[4];
			
		public:          
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
            void playFootStepSound() const;            
            /// <summary>
            /// Plays a random attack sound.
            /// </summary>
            /// <param name="a_HeavyAttack">Is it a heavy attack?</param>
            void playAttackSound(bool a_HeavyAttack) const;
            /// <summary>
            /// Plays a random grunt.
            /// </summary>
            void playRandomGrunt() const;            
            /// <summary>
            /// Plays a random sword swosh.
            /// </summary>
            void playRandomSwordSwosh() const;
            /// <summary>
            /// Updates the position of this instance.
            /// </summary>
            void updatePosition() const;            
            /// <summary>
            /// Creates the audio sources.
            /// </summary>
            void createAudioSources();
        };
    }
}

