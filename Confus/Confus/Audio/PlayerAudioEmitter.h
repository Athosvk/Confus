#pragma once
#include <irrlicht/irrlicht.h>
#include <vector>

#include "Sound.h"

namespace Confus
{
    namespace Audio
    {
		class AudioManager;

        /// <summary>
        /// Class PlayerAudioEmitter emits sounds like footsteps and attacking sounds from each individual player.
        /// </summary>
        class PlayerAudioEmitter
		{
			/// <summary> Sound u emit when hit </summary>
			Sound m_HitSound;
			/// <summary> The footstep sounds </summary>
			std::vector<Sound> m_Footsteps;
			/// <summary> The grunt sounds </summary>
			std::vector<Sound> m_Grunts;			
			/// <summary> The sword slash sounds </summary>
			std::vector<Sound> m_SwordSwoshes;            
			/// <summary> The player emmitting the sounds </summary>
			irr::scene::ISceneNode* m_AttachedPlayer;
        public:            
            /// <summary>
            /// Initializes a new instance of the <see cref="PlayerAudioEmitter"/> class.
            /// </summary>
            /// <param name="a_AttachedPlayer">The player that owns this emitter.</param>
            PlayerAudioEmitter(irr::scene::ISceneNode* a_AttachedPlayer, AudioManager* a_AudioManager);
            /// <summary>
            /// Plays a random footstep sound.
            /// </summary>
            void playFootStepSound();            
            /// <summary>
            /// Plays a random attack sound.
            /// </summary>
            /// <param name="a_HeavyAttack">Is it a heavy attack?</param>
            void playAttackSound(bool a_HeavyAttack);
            /// <summary>
            /// Plays a random grunt.
            /// </summary>
            void playRandomGrunt();            
            /// <summary>
            /// Plays a random sword swosh.
            /// </summary>
            void playRandomSwordSwosh();
			/// <summary>
			/// Plays a random sword swosh.
			/// </summary>
			void playHitSound();
			/// <summary>
            /// Updates the position of this instance.
            /// </summary>
            void updatePosition() const;            
            /// <summary>
            /// Creates the audio sources.
            /// </summary>
            void createAudioSources(AudioManager* a_AudioManager);
        };
    }
}

