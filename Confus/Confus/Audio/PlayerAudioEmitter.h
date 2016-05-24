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
			/// <summary> The sound played when you are hit by a heavy attack </summary>
			Sound m_HitSoundHeavy;			
			/// <summary>The sound played when you are hit by a ligth attack</summary>
			Sound m_HitSoundLight;			
			/// <summary>The sound played when you are hit by a heavy backstab</summary>
			Sound m_HitSoundHeavyBackstab;
			/// <summary>The sound played when you are hit by a light backstab</summary>
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
			/// <summary>Initializes a new instance of the <see cref="PlayerAudioEmitter" /> class.</summary>
			/// <param name="a_AttachedPlayer">The player that owns this emitter.</param>
			/// <param name="a_AudioManager">The audio manager the sounds should be played on</param>
/			PlayerAudioEmitter(ConfusShared::Player* a_AttachedPlayer, AudioManager* a_AudioManager);
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

			/// <summary>Plays a hit sound when you get hit.</summary>
			/// <param name="a_HitIdentifier">
			/// The hit identifier, to determine by what kind of attack the player was hit 
			/// </param>
			void playHitSound(EHitIdentifier a_HitIdentifier);
			/// <summary>
            /// Updates the position of this instance.
            /// </summary>
            void updatePosition();

			/// <summary>Creates the sounds for the audio emitter to play once a player event occrus</summary>
			/// <param name="a_AudioManager">The audio manager to create the sounds on</param>
			void createAudioSources(AudioManager* a_AudioManager);
        };
    }
}

