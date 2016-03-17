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
            delete(m_AudioSourceFootstep1);
            delete(m_AudioSourceFootstep2);
            delete(m_AudioSourceFootstep3);
            delete(m_AudioSourceGrunt1);
            delete(m_AudioSourceGrunt2);
            delete(m_AudioSourceSwordSwosh1);
            delete(m_AudioSourceSwordSwosh2);
            delete(m_AudioSourceSwordSwosh3);
            delete(m_AudioSourceSwordSwosh4);
            delete(m_AttachedPlayer);
        }

        void PlayerAudioEmitter::playFootStepSound()
        {
            if(!m_AudioSourceFootstep1->isPlaying())
            {
                m_AudioSourceFootstep1->play();
            }
            else if(!m_AudioSourceFootstep2->isPlaying())
            {
                m_AudioSourceFootstep2->play();
            }
            else
            {
                m_AudioSourceFootstep3->play();
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
                m_AudioSourceGrunt3->play();
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
                m_AudioSourceGrunt1->play();
                break;
            case 1:
                m_AudioSourceGrunt2->play();
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
                m_AudioSourceSwordSwosh1->play();
                break;
            case 1:
                m_AudioSourceSwordSwosh2->play();
                break;
            case 2:
                m_AudioSourceSwordSwosh3->play();
                break;
            case 3:
                m_AudioSourceSwordSwosh4->play();
                break;
            default:
                break;
            }
        }

        void PlayerAudioEmitter::updatePosition()
        {
            m_AudioSourceFootstep1->setPosition(m_AttachedPlayer->getAbsolutePosition());
            m_AudioSourceFootstep2->setPosition(m_AttachedPlayer->getAbsolutePosition());
            m_AudioSourceFootstep3->setPosition(m_AttachedPlayer->getAbsolutePosition());
        }

        void PlayerAudioEmitter::createAudioSources()
        {
            m_AudioSourceFootstep1 = new OpenALSource("Footstep1_Concrete.wav");
            m_AudioSourceFootstep2 = new OpenALSource("Footstep2_Concrete.wav");
            m_AudioSourceFootstep3 = new OpenALSource("Footstep3_Concrete.wav");

            m_AudioSourceGrunt1 = new OpenALSource("Grunt1.wav");
            m_AudioSourceGrunt2 = new OpenALSource("Grunt2.wav");
            m_AudioSourceGrunt3 = new OpenALSource("GruntHeavy.wav");
            m_AudioSourceGrunt1->setVolume(0.1f);
            m_AudioSourceGrunt2->setVolume(0.1f);
            m_AudioSourceGrunt3->setVolume(0.1f);


            m_AudioSourceSwordSwosh1 = new OpenALSource("Sword_swing_1.wav");
            m_AudioSourceSwordSwosh2 = new OpenALSource("Sword_swing_2.wav");
            m_AudioSourceSwordSwosh3 = new OpenALSource("Sword_swing_3.wav");
            m_AudioSourceSwordSwosh4 = new OpenALSource("Sword_swing_4.wav");
        }
    }
}
