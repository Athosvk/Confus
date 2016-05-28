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

	float CameraController::getYRotation() const
	{
		return m_YRotation;
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
		auto clampedX = irr::core::clamp(deltaPosition.Y, m_MinimumXRotation, m_MaximumXRotation);
		auto xRotation = fromAxisAngle(irr::core::vector3df(1.0f, 0.0f, 0.0f), clampedX * 
			irr::core::DEGTORAD);
		auto yRotation = fromAxisAngle(irr::core::vector3df(0.0f, 1.0f, 0.0f), deltaPosition.X * irr::core::DEGTORAD);
		m_Rotation *= yRotation * xRotation;

		irr::core::vector3df lookAt(0.0f, 0.0f, 1.0f);
		lookAt = m_Rotation * lookAt;
		m_CameraNode->setTarget(lookAt);
		m_YRotation += deltaPosition.X;
	}

	irr::core::quaternion CameraController::fromAxisAngle(irr::core::vector3df a_Axis, float a_Angle)
	{
		auto quaternion = irr::core::quaternion();
		quaternion.fromAngleAxis(a_Angle, a_Axis);
		return quaternion;
	}
}
