#include "PlayerAudioEmitter.h"

namespace Confus
{
    namespace Audio
    {
        PlayerAudioEmitter::PlayerAudioEmitter(irr::scene::IAnimatedMeshSceneNode* a_AttachedPlayer) : m_AttachedPlayer(a_AttachedPlayer)
        {
            m_AudioSource = new OpenALSource("Footstep1_Concrete.wav");
            playFootStepSound(); 
        }

        PlayerAudioEmitter::~PlayerAudioEmitter()
        {
            delete(m_AudioSource);
            delete(m_AttachedPlayer);
        }

        void PlayerAudioEmitter::playFootStepSound()
        {
            m_AudioSource->play();
        }

        void PlayerAudioEmitter::updatePosition()
        {
            m_AudioSource->setPosition(m_AttachedPlayer->getAbsolutePosition());
        }
    }
}
