#include <IrrAssimp/IrrAssimp.h>

#include "MoveableWall.h"
#include "Game.h"

namespace Confus
{
    MoveableWall::MoveableWall(irr::IrrlichtDevice* a_Device, irr::core::vector3df a_RegularPosition,
        irr::core::vector3df a_HiddenPosition, float a_TransitionSpeed)
        : m_RegularPosition(a_RegularPosition),
        m_HiddenPosition(a_HiddenPosition),
        m_TransitionSpeed(a_TransitionSpeed)
    {
        loadMesh(a_Device->getSceneManager());
        loadTextures(a_Device->getVideoDriver());
        m_Mesh->setPosition(m_RegularPosition);
        m_Mesh->setMaterialTexture(0, m_RegularTexture);
    }

    void MoveableWall::loadTextures(irr::video::IVideoDriver* a_VideoDriver)
    {
        m_RegularTexture = a_VideoDriver->getTexture("Media/Moveable wall/Concrete.png");
        m_TransparentTexture = a_VideoDriver->getTexture("Media/Moveable wall/Transparent.png");
    }

    void MoveableWall::loadMesh(irr::scene::ISceneManager* a_SceneManager)
    {
        IrrAssimp importer(a_SceneManager);
        m_Mesh = a_SceneManager->addAnimatedMeshSceneNode(importer.getMesh("Media/Moveable wall/Moveable wall.3DS"));
        m_Mesh->setMaterialType(irr::video::E_MATERIAL_TYPE::EMT_TRANSPARENT_ALPHA_CHANNEL);
    }

    void MoveableWall::hide()
    {
        m_TargetPosition = m_HiddenPosition;
        m_TransitionState = ETransitionState::Hiding;
    }

    void MoveableWall::rise()
    {
        m_TargetPosition = m_RegularPosition;
        m_TransitionState = ETransitionState::Rising;
        m_Mesh->setMaterialTexture(0, m_TransparentTexture);
    }

    void MoveableWall::fixedUpdate()
    {
        if(m_TransitionState != ETransitionState::Stationary)
        {
            updatePosition();
            if(m_TransitionState == ETransitionState::Rising)
            {
                updateTransparency();
            }
        }
    }

    void MoveableWall::updateTransparency()
    {
        auto distance = (m_TargetPosition - m_Mesh->getPosition()).getLength();
        if(distance > 0)
        {
            auto deltaDistance = distance / (m_HiddenPosition - m_RegularPosition).getLength();
            if(deltaDistance <= m_SolifyPoint)
            {
                m_Mesh->setMaterialTexture(0, m_RegularTexture);
            }
        }
    }

    void MoveableWall::updatePosition()
    {
        auto distance = (m_TargetPosition - m_Mesh->getPosition()).getLength();
        if(distance > 0.0)
        {
            auto clampedSpeed = irr::core::clamp(m_TransitionSpeed, 0.0f, distance);
            auto velocity = ((m_TargetPosition - m_Mesh->getPosition()) / distance) * clampedSpeed;
            m_Mesh->setPosition(m_Mesh->getPosition() + velocity);
        }
        else
        {
            m_TransitionState = ETransitionState::Stationary;
        }
    }
}
