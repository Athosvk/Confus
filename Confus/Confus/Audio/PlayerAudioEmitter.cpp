#include "PlayerAudioEmitter.h"

namespace Confus
{
    namespace Audio
    {
        PlayerAudioEmitter::PlayerAudioEmitter(Player a_AttachedPlayer) : m_AttachedPlayer(&a_AttachedPlayer)
        {
            m_AudioSource = new Confus::OpenALSource("SFX\Player\Footsteps\Concrete\Footstep1_Concrete.wav");
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
            m_AudioSource->setPosition(m_AttachedPlayer->getPosition());
        }
    }
}
