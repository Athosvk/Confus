#include <cmath>

#include "CameraController.h"

namespace Confus
{
	CameraController::CameraController(irr::IrrlichtDevice* a_Device, irr::scene::ICameraSceneNode* a_CameraNode,
		irr::scene::ISceneNode* m_OrbitNode)
		: m_Device(a_Device),
		m_CameraNode(a_CameraNode),
		m_OrbitNode(m_OrbitNode)
	{
		m_Device->getCursorControl()->setPosition(0.5f, 0.5f);
		m_Device->getCursorControl()->setVisible(false);
		updateTarget();
	}

	void CameraController::fixedUpdate()
	{
		if(m_Device->isWindowFocused())
		{
			wrapMouse();
			if(!m_Wrapped)
			{
				updateRotation();
				updateTarget();
			}
			m_PreviousPosition = m_Device->getCursorControl()->getRelativePosition();
			m_Wrapped = false;
		}
	}

	float CameraController::getYRotation() const
	{
		return m_Rotation.Y;
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

	void CameraController::updateRotation()
	{
		auto cursor = m_Device->getCursorControl();
		auto deltaPosition = (m_PreviousPosition - cursor->getRelativePosition()) * m_MouseSensitivity * m_AxesMultiplier;

		auto clampedXRotation = irr::core::clamp(deltaPosition.Y + m_Rotation.X,
			m_MinimumXRotation, m_MaximumXRotation);
		auto yRotation = m_Rotation.Y + deltaPosition.X;

		m_Rotation = irr::core::vector3df(clampedXRotation, yRotation, 0.0f);
	}

	void CameraController::updateTarget()
	{
		irr::core::vector3df target = m_Rotation.rotationToDirection() * 100000.0f;
		m_CameraNode->setTarget(target);
		m_CameraNode->updateAbsolutePosition();
	}
}
