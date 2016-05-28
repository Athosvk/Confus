#include <cmath>

#include "CameraController.h"

namespace Confus
{
	CameraController::CameraController(irr::IrrlichtDevice* a_Device, irr::scene::ICameraSceneNode* a_CameraNode,
		irr::scene::ISceneNode* a_DirectionNode)
		: m_Device(a_Device),
		m_CameraNode(a_CameraNode),
		m_DirectionNode(a_DirectionNode)
	{
		m_Device->getCursorControl()->setPosition(0.5f, 0.5f);
		m_Device->getCursorControl()->setVisible(false);
	}

	void CameraController::update()
	{
		if(m_Device->isWindowFocused())
		{
			wrapMouse();
		}
		auto cursor = m_Device->getCursorControl();
		if(!m_Wrapped)
		{
			auto deltaPosition = m_PreviousPosition - cursor->getRelativePosition();
			auto rotationY = m_DirectionNode->getRotation().Y;
			rotationY += deltaPosition.X * -100.0f;
			m_DirectionNode->setRotation(irr::core::vector3df(m_DirectionNode->getRotation().X, rotationY, 
				m_DirectionNode->getRotation().Z));

			m_XRotation = irr::core::clamp(m_XRotation + deltaPosition.Y * -100.0f, -60.0f, 60.0f);
			
			irr::core::vector3df lookAt(0.0f, 0.0f, 1.0f);
			lookAt.rotateXZBy(-rotationY);
			lookAt.rotateYZBy(m_XRotation);
			m_CameraNode->setTarget(lookAt);
		}
		m_PreviousPosition = cursor->getRelativePosition();
		m_Wrapped = false;
	}

	void CameraController::wrapMouse()
	{
		auto cursor = m_Device->getCursorControl();
		auto position = cursor->getRelativePosition();
		if(position.X >= 1.0f || position.X <= 0.00f)
		{
			position.X = 0.5f;
			m_Wrapped = true;
		}
		if(position.Y >= 1.0f || position.Y <= 0.00f)
		{
			position.Y = 0.5f;
			m_Wrapped = true;
		}
		m_Device->getCursorControl()->setPosition(position.X, position.Y);
	}
}
