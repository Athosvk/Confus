#include <time.h> 

#include "PlayerAudioEmitter.h"
#include "AudioManager.h"

namespace Confus
{
    namespace Audio
    {
        PlayerAudioEmitter::PlayerAudioEmitter(irr::scene::ISceneNode* a_AttachedPlayer, AudioManager* a_AudioManager)
			: m_AttachedPlayer(a_AttachedPlayer)
        {
			createAudioSources(a_AudioManager);
        }

        void PlayerAudioEmitter::playFootStepSound()
        {
            if(!m_AudioSourceFootsteps[0].isPlaying())
            {
                m_AudioSourceFootsteps[0].play();
            }
            else if(!m_AudioSourceFootsteps[1].isPlaying())
            {
                m_AudioSourceFootsteps[1].play();
            }
            else
            {
                m_AudioSourceFootsteps[2].play();
            }
        }

        void PlayerAudioEmitter::playAttackSound(bool a_HeavyAttack)
        {
            if(!a_HeavyAttack)
            {
                playRandomGrunt();
            }
            else
            {
                m_AudioSourceGrunts[2].play();
            }
            playRandomSwordSwosh();
        }

        void PlayerAudioEmitter::playRandomGrunt()
        {
            std::srand(static_cast<int>(time(NULL)));
			m_AudioSourceGrunts[std::rand() % 2].play();
        }

        void PlayerAudioEmitter::playRandomSwordSwosh()
        {
			std::srand(static_cast<int>(time(NULL)));
			m_AudioSourceSwordSwoshes[std::rand() % 4].play();
        }

        void PlayerAudioEmitter::updatePosition() const
        {
            m_AttachedPlayer->updateAbsolutePosition();
            for(auto sound : m_AudioSourceFootsteps)
            {
                sound.setPosition(m_AttachedPlayer->getAbsolutePosition());

                irr::core::matrix4 playerRotation(m_AttachedPlayer->getAbsoluteTransformation());
                irr::core::vector3df forwardVector = irr::core::vector3df(playerRotation[8], playerRotation[9], playerRotation[10]);
                irr::core::vector3df upVector = irr::core::vector3df(playerRotation[4], playerRotation[5], playerRotation[6]);
                sound.setDirection(forwardVector, upVector);
            }

            for (auto sound : m_AudioSourceGrunts)
            {
				sound.setPosition(m_AttachedPlayer->getAbsolutePosition());

                irr::core::matrix4 playerRotation(m_AttachedPlayer->getAbsoluteTransformation());
                irr::core::vector3df forwardVector = irr::core::vector3df(playerRotation[8], playerRotation[9], playerRotation[10]);
                irr::core::vector3df upVector = irr::core::vector3df(playerRotation[4], playerRotation[5], playerRotation[6]);
                sound.setDirection(forwardVector, upVector);
            }

            for(auto sound : m_AudioSourceSwordSwoshes)
            {
                sound.setPosition(m_AttachedPlayer->getAbsolutePosition());

                irr::core::matrix4 playerRotation(m_AttachedPlayer->getAbsoluteTransformation());
                irr::core::vector3df forwardVector = irr::core::vector3df(playerRotation[8], playerRotation[9], playerRotation[10]);
                irr::core::vector3df upVector = irr::core::vector3df(playerRotation[4], playerRotation[5], playerRotation[6]);
                sound.setDirection(forwardVector, upVector);
            }
        }

        void PlayerAudioEmitter::createAudioSources(AudioManager* a_AudioManager)
        {
            for(int i = 0; i < 4; i++)
            {
                if(i < 3)
                {
                    m_AudioSourceFootsteps.push_back(a_AudioManager->createSound("Footstep" + std::to_string(i + 1) + "_Concrete.wav"));
                }
                if(i < 2)
                {
                    m_AudioSourceGrunts.push_back(a_AudioManager->createSound("Grunt" + std::to_string(i + 1) + ".wav"));
                }
                m_AudioSourceSwordSwoshes.push_back(a_AudioManager->createSound("Sword_swing_" + std::to_string(i + 1) + ".wav"));
            }

            m_AudioSourceGrunts.push_back(a_AudioManager->createSound("GruntHeavy.wav"));

            for(auto sound : m_AudioSourceGrunts)
            {
				sound.setVolume(0.1f);
            }
        }
    }
}
