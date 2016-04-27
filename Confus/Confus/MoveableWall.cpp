#include <IrrAssimp/IrrAssimp.h>

#include "MoveableWall.h"
#include "Game.h"
#include "../ConfusShared/Physics/BoxCollider.h"
#include "../ConfusShared/Physics/PhysicsWorld.h"

namespace Confus
{

	MoveableWall::MoveableWall(irr::IrrlichtDevice* a_Device, irr::core::vector3df a_RegularPosition,
		irr::core::vector3df a_Scale, Physics::PhysicsWorld& a_PhysicsWorld)
        : m_RegularPosition(a_RegularPosition)
	{
        loadMesh(a_Device->getSceneManager());
        loadTextures(a_Device->getVideoDriver());
        m_MeshNode->setPosition(m_RegularPosition);
		m_MeshNode->setScale(a_Scale);
		m_RigidBody = a_PhysicsWorld.createBoxCollider(m_MeshNode, Physics::ECollisionFilter::MoveableWall,
			Physics::ECollisionFilter::Player)->getRigidBody();
		m_RigidBody->makeKinematic();
        solidify();
    }

    MoveableWall::~MoveableWall()
    {
    }

    void MoveableWall::loadTextures(irr::video::IVideoDriver* a_VideoDriver)
    {
        m_RegularTexture = a_VideoDriver->getTexture("Media/Textures/SquareWall.jpg");
        m_TransparentTexture = a_VideoDriver->getTexture("Media/Textures/SquareWallTransparent.png");
    }

    void MoveableWall::loadMesh(irr::scene::ISceneManager* a_SceneManager)
    {
        m_MeshNode = a_SceneManager->addAnimatedMeshSceneNode(a_SceneManager->getMesh("Media/Meshes/WallMeshSquare.irrmesh"));
    }

    void MoveableWall::hide()
    {
        m_TargetPosition = HiddenPosition;
        m_Transitioning = true;
		makeTransparent();
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
		m_MeshNode->setMaterialType(irr::video::E_MATERIAL_TYPE::EMT_SOLID);
    }

    void MoveableWall::makeTransparent()
    {
		m_MeshNode->setMaterialType(irr::video::E_MATERIAL_TYPE::EMT_TRANSPARENT_ALPHA_CHANNEL);
        m_MeshNode->setMaterialTexture(0, m_TransparentTexture);
        disableCollision();
    }

    void MoveableWall::enableCollision()
    {
		m_RigidBody->activate();
    }

    void MoveableWall::disableCollision()
    {
		m_RigidBody->deactivate();
    }

    void MoveableWall::updatePosition()
    {
        auto distance = (m_TargetPosition - m_MeshNode->getPosition()).getLength();
        if(distance > 0.01f)
        {
            auto clampedSpeed = irr::core::clamp(TransitionSpeed, 0.0f, distance);
            auto velocity = ((m_TargetPosition - m_MeshNode->getPosition()) / distance) * clampedSpeed;
            m_MeshNode->setPosition(m_MeshNode->getPosition() + velocity);
        }
        else if(m_Raised)
        {
            m_Transitioning = false;
			m_MeshNode->setVisible(false);
			m_Raised = false;
        }
		else if (!m_Raised)
		{
			m_Raised = true;
			m_Transitioning = false;
		}
    }
}
