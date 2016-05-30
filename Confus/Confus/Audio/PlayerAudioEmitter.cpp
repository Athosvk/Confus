#include <time.h> 
#include <algorithm>

#include "PlayerAudioEmitter.h"
#include "AudioManager.h"
#include "../../ConfusShared/Player.h"

namespace Confus
{
    namespace Audio
    {
		PlayerAudioEmitter::PlayerAudioEmitter(ConfusShared::Player* a_AttachedPlayer, AudioManager* a_AudioManager)
			: m_HitSoundHeavy(a_AudioManager->createSound("SFX/Player/a_heavy_grunt.wav")),
			m_HitSoundLight(a_AudioManager->createSound("SFX/Player/a_light_grunt.wav")),
			m_HitSoundHeavyBackstab(a_AudioManager->createSound("SFX/Player/a_heavybackstab_grunt.wav")),
			m_HitSoundLightBackstab(a_AudioManager->createSound("SFX/Player/a_lightbackstab_grunt.wav")),
			m_AttachedPlayer(a_AttachedPlayer)
        {
			createAudioSources(a_AudioManager);
			m_AttachedPlayer->getHealthInstance()->DamageEvent += [this](EHitIdentifier a_HitIdentifier) -> void
			{
				//playHitSound(a_HitIdentifier);
			};

			m_AttachedPlayer->OnHeavyAttack += [this]() -> void
			{
				//playHeavyAttack();
			};

			m_AttachedPlayer->OnLightAttack += [this]() -> void
			{
				//playLightAttack();
			};
        }

        void PlayerAudioEmitter::playFootStepSound()
        {
			auto soundIterator = std::find_if(m_Footsteps.begin(), m_Footsteps.end(), [](const Sound& a_Sound)->bool
			{
				return !a_Sound.isPlaying();
			});
			
			if (soundIterator != m_Footsteps.end())
			{
				//soundIterator->play();
			}
        }

		void PlayerAudioEmitter::playHeavyAttack()
		{
			//m_Footsteps[2].play();
			//playRandomSwordSwosh();
		}

        void PlayerAudioEmitter::playLightAttack()
        {
			//playRandomGrunt();
            //playRandomSwordSwosh();
        }

        void PlayerAudioEmitter::playRandomGrunt()
        {
            //std::srand(static_cast<int>(time(nullptr)));
			//m_Grunts[std::rand() % 2].play();
        }

        void PlayerAudioEmitter::playRandomSwordSwosh()
        {
            //std::srand(static_cast<int>(time(nullptr)));
			m_SwordSwoshes[std::rand() % 4].play();
        }

		void PlayerAudioEmitter::playHitSound(EHitIdentifier a_HitIdentifier )
		{
			if (a_HitIdentifier == EHitIdentifier::Heavy)
			{
				m_HitSoundHeavy.play();
			}
			else if (a_HitIdentifier == EHitIdentifier::Light)
			{
				m_HitSoundLight.play();
			}
			else if (a_HitIdentifier == EHitIdentifier::LightBackstab)
			{
				m_HitSoundLightBackstab.play();
			}
			else if (a_HitIdentifier == EHitIdentifier::HeavyBackstab)
			{
				m_HitSoundHeavyBackstab.play();
			}
		}

        void PlayerAudioEmitter::updatePosition()
        {
            m_AttachedPlayer->updateAbsolutePosition();

			int frameNumber = static_cast<int>(m_AttachedPlayer->getAnimationFrame());
			if(frameNumber == 0 || frameNumber == 6)
			{
				playFootStepSound();
			}

			irr::core::matrix4 playerRotation = m_AttachedPlayer->getAbsoluteTransformation();
			irr::core::vector3df forwardVector = irr::core::vector3df(playerRotation[8], playerRotation[9], playerRotation[10]);
			irr::core::vector3df upVector = irr::core::vector3df(playerRotation[4], playerRotation[5], playerRotation[6]);
            for(auto sound : m_Footsteps)
            {
                sound.setPosition(m_AttachedPlayer->getAbsolutePosition());
                sound.setDirection(forwardVector, upVector);
            }

            for(auto sound : m_Grunts)
            {
				sound.setPosition(m_AttachedPlayer->getAbsolutePosition());
                sound.setDirection(forwardVector, upVector);
            }

            for(auto sound : m_SwordSwoshes)
            {
                sound.setPosition(m_AttachedPlayer->getAbsolutePosition());
                sound.setDirection(forwardVector, upVector);
            }
        }

		void PlayerAudioEmitter::createAudioSources(AudioManager* a_AudioManager)
        {
            for(int i = 0; i < 4; i++)
            {
                if(i < 3)
                {
					m_Footsteps.push_back(a_AudioManager->createSound("Footstep" + std::to_string(i + 1) + "_Concrete.wav"));
                }
                if(i < 2)
                {
                    m_Grunts.push_back(a_AudioManager->createSound("Grunt" + std::to_string(i + 1) + ".wav"));
                }
                m_SwordSwoshes.push_back(a_AudioManager->createSound("Sword_swing_" + std::to_string(i + 1) + ".wav"));
            }

            m_Grunts.push_back(a_AudioManager->createSound("GruntHeavy.wav"));

            for(auto sound : m_Grunts)
            {
				sound.setVolume(0.1f);
            }
        }
    }
}
