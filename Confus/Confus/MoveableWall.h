#pragma once
#include <Irrlicht/irrlicht.h>

namespace Confus
{
    class MoveableWall
    {
    private:
        enum class ETransitionState
        {
            Hiding,
            Rising,
            Stationary
        } 
        m_TransitionState = ETransitionState::Stationary;
        irr::scene::IAnimatedMeshSceneNode* m_Mesh;
        irr::core::vector3d<float> m_RegularPosition;
        irr::core::vector3d<float> m_HiddenPosition;
        irr::core::vector3d<float> m_TargetPosition;
        float m_TransitionSpeed;
        float m_SolifyPoint = 0.1f;
        irr::video::ITexture* m_RegularTexture;
        irr::video::ITexture* m_TransparentTexture;

    public:
        MoveableWall(irr::IrrlichtDevice* a_Device, irr::core::vector3df a_RegularPosition, 
            irr::core::vector3df a_HiddenPosition, float a_TransitionSpeed = 0.1f);

        void hide();
        void rise();
        void fixedUpdate();
    private:
        void updatePosition();
        void updateTransparency();
        void setTransparency(irr::u32 a_Transparency);
    };
}

