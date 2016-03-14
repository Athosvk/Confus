#pragma once
#include <irrlicht/irrlicht.h>

#include "Audio\PlayerAudioEmitter.h"
namespace Confus
{
    namespace Audio {
        class PlayerAudioEmitter;
    }
    
    class Player
    {
    private:
        Audio::PlayerAudioEmitter* m_FootstepSoundEmitter;

        void createAudioEmitter();
    public:
        irr::scene::IAnimatedMeshSceneNode* PlayerNode;
        Player(irr::IrrlichtDevice* a_Device);
        void fixedUpdate();
        void update();
    };
}