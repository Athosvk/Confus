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
        auto sceneManager = a_Device->getSceneManager();
        IrrAssimp importer(sceneManager);
        m_Mesh = sceneManager->
            addAnimatedMeshSceneNode(importer.getMesh("Media/Moveable wall/Moveable wall.3DS"));
        auto videoDriver = a_Device->getVideoDriver();
        m_RegularTexture = videoDriver->getTexture("Media/Moveable wall/Concrete.png");
        m_TransparentTexture = videoDriver->getTexture("Media/Moveable wall/Transparent.png");
        m_Mesh->setPosition(m_RegularPosition);
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
