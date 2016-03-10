#pragma once
#include <irrlicht/irrlicht.h>

#include "..\Player.h"

namespace Confus
{
    namespace Audio
    {
        class PlayerAudioEmitter
        {
        public:
            PlayerAudioEmitter(Player a_Player);
            ~PlayerAudioEmitter();

            void PlayFootStepSound();
            void PlayAttackSound();

        };
    }
}

