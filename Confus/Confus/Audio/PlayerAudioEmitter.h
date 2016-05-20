#pragma once
#include <vector>

#include "Sound.h"
#include "../../ConfusShared/EHitIdentifier.h"

namespace ConfusShared
{
	class Player;
}

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
			Sound m_HitSoundHeavy;
			Sound m_HitSoundLight;
			Sound m_HitSoundHeavyBackstab;
			Sound m_HitSoundLightBackstab;
			/// <summary> The footstep sounds </summary>
			std::vector<Sound> m_Footsteps;
			/// <summary> The grunt sounds </summary>
			std::vector<Sound> m_Grunts;			
			/// <summary> The sword slash sounds </summary>
			std::vector<Sound> m_SwordSwoshes;            
			/// <summary> The player emmitting the sounds </summary>
			ConfusShared::Player* m_AttachedPlayer;
        public:            
            /// <summary>
            /// Initializes a new instance of the <see cref="PlayerAudioEmitter"/> class.
            /// </summary>
            /// <param name="a_AttachedPlayer">The player that owns this emitter.</param>
            PlayerAudioEmitter(ConfusShared::Player* a_AttachedPlayer, AudioManager* a_AudioManager);
            /// <summary>
            /// Plays a random footstep sound.
            /// </summary>
            void playFootStepSound();			
			/// <summary> Plays the heavy attack sound.</summary>
			void playHeavyAttack();
            /// <summary> Plays the light attack sound. </summary>
            void playLightAttack();
            /// <summary>
            /// Plays a random grunt.
            /// </summary>
            void playRandomGrunt();            
            /// <summary>
            /// Plays a random sword swosh.
            /// </summary>
            void playRandomSwordSwosh();
			/// <summary>
			/// Plays a hit sound when you get hit.
			/// </summary>
			void playHitSound(EHitIdentifier a_HitIdentifier);
			/// <summary>
            /// Updates the position of this instance.
            /// </summary>
            void updatePosition();            
            /// <summary>
            /// Creates the audio sources.
            /// </summary>
            void createAudioSources(AudioManager* a_AudioManager);
        };
    }
}

