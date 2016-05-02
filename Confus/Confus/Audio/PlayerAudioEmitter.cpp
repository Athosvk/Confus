#include <time.h> 

#include "PlayerAudioEmitter.h"
#include "AudioManager.h"

namespace Confus
{
    namespace Audio
    {
        PlayerAudioEmitter::PlayerAudioEmitter(irr::scene::IAnimatedMeshSceneNode* a_AttachedPlayer, AudioManager* a_AudioManager)
			: m_AttachedPlayer(a_AttachedPlayer)
        {
			createAudioSources(a_AudioManager);
        }

        PlayerAudioEmitter::~PlayerAudioEmitter()
        {
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
            auto randomNumber = std::rand() % 2;

            switch(randomNumber)
            {
            case 0:
                m_AudioSourceGrunts[0].play();
                break;
            case 1:
                m_AudioSourceGrunts[1].play();
                break;
            default:
                break;
            }
        }

        void PlayerAudioEmitter::playRandomSwordSwosh()
        {
            std::srand(static_cast<int>(time(NULL)));
            auto randomNumber = std::rand() % 4;

            switch(randomNumber)
            {
            case 0:
                m_AudioSourceSwordSwoshes[0].play();
                break;
            case 1:
                m_AudioSourceSwordSwoshes[1].play();
                break;
            case 2:
                m_AudioSourceSwordSwoshes[2].play();
                break;
            case 3:
                m_AudioSourceSwordSwoshes[3].play();
                break;
            default:
                break;
            }
        }

        void PlayerAudioEmitter::updatePosition() const
        {
            for(auto sound : m_AudioSourceFootsteps)
            {
                sound.setPosition(m_AttachedPlayer->getAbsolutePosition());
            }

            for (auto sound : m_AudioSourceGrunts)
            {
				sound.setPosition(m_AttachedPlayer->getAbsolutePosition());
            }

            for(auto sound : m_AudioSourceSwordSwoshes)
            {
                sound.setPosition(m_AttachedPlayer->getAbsolutePosition());
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
