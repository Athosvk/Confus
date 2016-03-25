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
        }
    }
}
