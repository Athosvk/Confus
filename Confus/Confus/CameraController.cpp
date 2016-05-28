#include <cmath>

#include "CameraController.h"

namespace Confus
{
	CameraController::CameraController(irr::IrrlichtDevice* a_Device, irr::scene::ICameraSceneNode* a_CameraNode)
		: m_Device(a_Device),
		m_CameraNode(a_CameraNode)
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
		if(!m_Wrapped)
		{
			updateTarget();
		}
		m_PreviousPosition = m_Device->getCursorControl()->getRelativePosition();
		m_Wrapped = false;
	}

	irr::core::vector3df CameraController::getRotation() const
	{
		return m_Rotation;
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

	void CameraController::updateTarget()
	{
		auto cursor = m_Device->getCursorControl();
		auto deltaPosition = (m_PreviousPosition - cursor->getRelativePosition()) * m_MouseSensitivity * m_AxesMultiplier;
		irr::core::vector3df rotation = m_Rotation;

		rotation.Y += deltaPosition.X;
		rotation.X = irr::core::clamp(rotation.X + deltaPosition.Y, m_MinimumXRotation, m_MaximumXRotation);

		irr::core::vector3df lookAt(0.0f, 0.0f, 1.0f);
		lookAt.rotateXZBy(rotation.Y);
		lookAt.rotateYZBy(rotation.X);
		m_CameraNode->setTarget(lookAt);

		m_Rotation = rotation;
	}
}
