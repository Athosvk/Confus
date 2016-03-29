#include <IrrAssimp/IrrAssimp.h>

#include "MoveableWall.h"
#include "Game.h"

namespace Confus
{
    MoveableWall::MoveableWall(irr::IrrlichtDevice* a_Device, irr::core::vector3df a_RegularPosition,
        irr::core::vector3df a_HiddenPosition)
        : m_RegularPosition(a_RegularPosition),
        HiddenPosition(a_HiddenPosition)
    {
        loadMesh(a_Device->getSceneManager());
        loadTextures(a_Device->getVideoDriver());
        m_MeshNode->setPosition(m_RegularPosition);
        solidify();
    }

    MoveableWall::~MoveableWall()
    {
        //m_MeshNode->drop();
        //m_TriangleSelector->drop();
        //m_RegularTexture->drop();
        //m_TransparentTexture->drop();
    }

    void MoveableWall::loadTextures(irr::video::IVideoDriver* a_VideoDriver)
    {
        m_RegularTexture = a_VideoDriver->getTexture("Media/Textures/SquareWall.jpg");
        m_TransparentTexture = a_VideoDriver->getTexture("Media/Textures/SquareWallTransparent.png");
    }

    void MoveableWall::loadMesh(irr::scene::ISceneManager* a_SceneManager)
    {
        IrrAssimp importer(a_SceneManager);
        m_MeshNode = a_SceneManager->addAnimatedMeshSceneNode(a_SceneManager->getMesh("Media/Meshes/WallMeshSquare.irrmesh"));
        m_TriangleSelector = a_SceneManager->createTriangleSelector(m_MeshNode);
    }

    void MoveableWall::hide()
    {
		m_MeshNode->setMaterialType(irr::video::E_MATERIAL_TYPE::EMT_TRANSPARENT_ALPHA_CHANNEL);
        m_TargetPosition = HiddenPosition;
        m_Transitioning = true;
    }

    void MoveableWall::rise()
    {
        m_TargetPosition = m_RegularPosition;
        m_Transitioning = true;
		m_MeshNode->setVisible(true);
    }

    void MoveableWall::fixedUpdate()
    {
        if(m_Transitioning)
        {
            updatePosition();
            updateTransparency();
        }
    }

    void MoveableWall::updateTransparency()
    {
        auto distance = (HiddenPosition - m_MeshNode->getPosition()).getLength();
        auto deltaDistance = distance / (HiddenPosition - m_RegularPosition).getLength();
        if(deltaDistance >= SolifyPoint)
        {
            solidify();
        }
        else
        {
            makeTransparent();
        }
    }

    void MoveableWall::solidify()
    {
        m_MeshNode->setMaterialTexture(0, m_RegularTexture);
        enableCollision();
    }

    void MoveableWall::makeTransparent()
    {
        m_MeshNode->setMaterialTexture(0, m_TransparentTexture);
        disableCollision();
    }

    void MoveableWall::enableCollision()
    {
        m_MeshNode->setTriangleSelector(m_TriangleSelector);
    }

    void MoveableWall::disableCollision()
    {
        m_MeshNode->setTriangleSelector(nullptr);
    }

    void MoveableWall::updatePosition()
    {
        auto distance = (m_TargetPosition - m_MeshNode->getPosition()).getLength();
        if(distance > 0.0f)
        {
            auto clampedSpeed = irr::core::clamp(TransitionSpeed, 0.0f, distance);
            auto velocity = ((m_TargetPosition - m_MeshNode->getPosition()) / distance) * clampedSpeed;
            m_MeshNode->setPosition(m_MeshNode->getPosition() + velocity);
        }
        else if(m_raised)
        {
            m_Transitioning = false;
			m_MeshNode->setVisible(false);
			m_raised = false;
        }
		else if (!m_raised)
		{
			m_raised = true;
			m_Transitioning = false;
			m_MeshNode->setMaterialType(irr::video::E_MATERIAL_TYPE::EMT_SOLID);
		}
    }
}
