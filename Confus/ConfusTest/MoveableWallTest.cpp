#include "stdafx.h"

#include <ConfusShared/MoveableWall.h>
#include <ConfusShared/Physics/PhysicsWorld.h>

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ConfusTest
{
	TEST_CLASS(MoveableWallTest)
	{
		irr::IrrlichtDevice* m_Device = irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_NULL);
		irr::core::vector3df m_HiddenPosition = irr::core::vector3df(0.0f);
		irr::core::vector3df m_StartPosition = irr::core::vector3df(0.0f, 1.0f, 0.0f);
		irr::core::vector3df m_MidPoint = irr::core::vector3df(0.0f, 0.5f, 0.0f);

	public:
		TEST_METHOD(testHiding)
		{
			ConfusShared::Physics::PhysicsWorld physicsWorld(m_Device);
			ConfusShared::MoveableWall moveableWall =
				ConfusShared::MoveableWall(m_Device, m_StartPosition, m_HiddenPosition, physicsWorld);
			moveableWall.TransitionSpeed = 0.5f;
			moveableWall.hide();
			moveableWall.fixedUpdate();
			Assert::AreNotEqual(moveableWall.getMeshNode()->getPosition().Y, m_HiddenPosition.Y, 0.01f);
			moveableWall.fixedUpdate();
			Assert::AreEqual(moveableWall.getMeshNode()->getPosition().Y, m_HiddenPosition.Y, 0.01f);
		}

		TEST_METHOD(testRising)
		{
			ConfusShared::Physics::PhysicsWorld physicsWorld(m_Device);
			ConfusShared::MoveableWall moveableWall =
				ConfusShared::MoveableWall(m_Device, m_StartPosition, m_HiddenPosition, physicsWorld);
			moveableWall.TransitionSpeed = 0.5f;
			moveableWall.hide();
			moveableWall.fixedUpdate();
			moveableWall.fixedUpdate();
			moveableWall.rise();
			moveableWall.fixedUpdate();
			Assert::AreNotEqual(moveableWall.getMeshNode()->getPosition().Y, m_StartPosition.Y, 0.01f);
			moveableWall.fixedUpdate();
			Assert::AreEqual(moveableWall.getMeshNode()->getPosition().Y, m_StartPosition.Y, 0.01f);
		}

		TEST_METHOD(testHalfway)
		{
			ConfusShared::Physics::PhysicsWorld physicsWorld(m_Device);
			ConfusShared::MoveableWall moveableWall =
				ConfusShared::MoveableWall(m_Device, m_StartPosition, m_HiddenPosition, physicsWorld);
			moveableWall.TransitionSpeed = 0.5f;
			moveableWall.hide();
			moveableWall.fixedUpdate();
			Assert::AreEqual(moveableWall.getMeshNode()->getPosition().Y, m_MidPoint.Y, 0.01f);
			moveableWall.fixedUpdate();
			Assert::AreNotEqual(moveableWall.getMeshNode()->getPosition().Y, m_MidPoint.Y, 0.01f);
			moveableWall.rise();
			moveableWall.fixedUpdate();
			Assert::AreEqual(moveableWall.getMeshNode()->getPosition().Y, m_MidPoint.Y, 0.01f);
			moveableWall.fixedUpdate();
			Assert::AreNotEqual(moveableWall.getMeshNode()->getPosition().Y, m_MidPoint.Y, 0.01f);
		}

		TEST_METHOD(testOvershoot)
		{
			ConfusShared::Physics::PhysicsWorld physicsWorld(m_Device);
			ConfusShared::MoveableWall moveableWall = ConfusShared::MoveableWall(m_Device,
				m_StartPosition, m_HiddenPosition, physicsWorld);
			moveableWall.TransitionSpeed = 15.0f;
			moveableWall.hide();
			moveableWall.fixedUpdate();
			Assert::AreEqual(moveableWall.getMeshNode()->getPosition().Y, m_HiddenPosition.Y, 0.01f);
			moveableWall.rise();
			moveableWall.fixedUpdate();			
			Assert::AreEqual(moveableWall.getMeshNode()->getPosition().Y, m_StartPosition.Y, 0.01f);
		}
	};
}