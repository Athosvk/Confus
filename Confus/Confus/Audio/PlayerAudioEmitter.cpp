#include <time.h> 

#include "PlayerAudioEmitter.h"

namespace Confus
{
    namespace Audio
    {
        PlayerAudioEmitter::PlayerAudioEmitter(irr::scene::IAnimatedMeshSceneNode* a_AttachedPlayer) : m_AttachedPlayer(a_AttachedPlayer)
        {
createAudioSources();
        }

        PlayerAudioEmitter::~PlayerAudioEmitter()
        {
            for(auto audioSource : m_AudioSourceFootsteps)
            {
                delete(audioSource);
            }
            for(auto audioSource : m_AudioSourceGrunts)
            {
                delete(audioSource);
            }
            for(auto audioSource : m_AudioSourceSwordSwoshes)
            {
                delete(audioSource);
            }
        }

        void PlayerAudioEmitter::playFootStepSound() const
        {
            if(!m_AudioSourceFootsteps[0]->isPlaying())
            {
                m_AudioSourceFootsteps[0]->play();
            }
            else if(!m_AudioSourceFootsteps[1]->isPlaying())
            {
                m_AudioSourceFootsteps[1]->play();
            }
            else
            {
                m_AudioSourceFootsteps[2]->play();
            }
        }


        void PlayerAudioEmitter::playAttackSound(bool a_HeavyAttack) const
        {
            if(!a_HeavyAttack)
            {
                playRandomGrunt();
            }
            else
            {
                m_AudioSourceGrunts[2]->play();
            }
            playRandomSwordSwosh();
        }

        void PlayerAudioEmitter::playRandomGrunt() const
        {
            std::srand(static_cast<int>(time(NULL)));
            auto randomNumber = std::rand() % 2;

            switch(randomNumber)
            {
            case 0:
                m_AudioSourceGrunts[0]->play();
                break;
            case 1:
                m_AudioSourceGrunts[1]->play();
                break;
            default:
                break;
            }
        }

        void PlayerAudioEmitter::playRandomSwordSwosh() const
        {
            std::srand(static_cast<int>(time(NULL)));
            auto randomNumber = std::rand() % 4;

            switch(randomNumber)
            {
            case 0:
                m_AudioSourceSwordSwoshes[0]->play();
                break;
            case 1:
                m_AudioSourceSwordSwoshes[1]->play();
                break;
            case 2:
                m_AudioSourceSwordSwoshes[2]->play();
                break;
            case 3:
                m_AudioSourceSwordSwoshes[3]->play();
                break;
            default:
                break;
            }
        }

        void PlayerAudioEmitter::updatePosition() const
        {
            for(auto audioSource : m_AudioSourceFootsteps)
            {
                audioSource->setPosition(m_AttachedPlayer->getAbsolutePosition());
            }

            for (auto audioSource : m_AudioSourceGrunts)
            {
                audioSource->setPosition(m_AttachedPlayer->getAbsolutePosition());
            }
        }

        void PlayerAudioEmitter::createAudioSources()
        {
            m_AudioSourceFootsteps[0] = new OpenALSource("Footstep1_Concrete.wav");
            m_AudioSourceFootsteps[1] = new OpenALSource("Footstep2_Concrete.wav");
            m_AudioSourceFootsteps[2] = new OpenALSource("Footstep3_Concrete.wav");

            m_AudioSourceGrunts[0] = new OpenALSource("Grunt1.wav");
            m_AudioSourceGrunts[1] = new OpenALSource("Grunt2.wav");
            m_AudioSourceGrunts[2] = new OpenALSource("GruntHeavy.wav");

            for(auto audioSource : m_AudioSourceGrunts)
            {
                audioSource->setVolume(0.1f);
            }
            
            m_AudioSourceSwordSwoshes[0] = new OpenALSource("Sword_swing_1.wav");
            m_AudioSourceSwordSwoshes[1] = new OpenALSource("Sword_swing_2.wav");
            m_AudioSourceSwordSwoshes[2] = new OpenALSource("Sword_swing_3.wav");
            m_AudioSourceSwordSwoshes[3] = new OpenALSource("Sword_swing_4.wav");
        }
    }
}
