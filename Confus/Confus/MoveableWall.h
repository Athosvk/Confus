#pragma once
#include <Irrlicht/irrlicht.h>

namespace Confus
{
    class MoveableWall
    {
    public:
        float TransitionSpeed = 0.1f;
        float SolifyPoint = 0.2f;
    private:
        irr::scene::IAnimatedMeshSceneNode* m_MeshNode;
        irr::core::vector3d<float> m_RegularPosition;
        irr::core::vector3d<float> m_HiddenPosition;
        irr::core::vector3d<float> m_TargetPosition;
        irr::video::ITexture* m_RegularTexture;
        irr::video::ITexture* m_TransparentTexture;
        bool m_Transitioning = false;

    public:
        MoveableWall(irr::IrrlichtDevice* a_Device, irr::core::vector3df a_RegularPosition, 
            irr::core::vector3df a_HiddenPosition);

        void hide();
        void rise();
        void fixedUpdate();
    private:
        void loadTextures(irr::video::IVideoDriver* a_VideoDriver);
        void loadMesh(irr::scene::ISceneManager* a_SceneManager);
        void updatePosition();
        void updateTransparency();
    };
}