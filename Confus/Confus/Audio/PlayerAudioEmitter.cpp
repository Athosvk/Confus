#include "PlayerAudioEmitter.h"

namespace Confus
{
    namespace Audio
    {
        PlayerAudioEmitter::PlayerAudioEmitter(Player a_AttachedPlayer) : m_AttachedPlayer(&a_AttachedPlayer)
        {
            Confus::OpenALSource* m_AudioSource = new Confus::OpenALSource("Footsteps.wav");
            m_AudioSource->play();
            // Not working yet because m_AudioSource goes out of scope. 
            //playFootStepSound(); 
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
//            m_AudioSource->setPosition(m_AttachedPlayer->getPosition());
        }
    }
}
