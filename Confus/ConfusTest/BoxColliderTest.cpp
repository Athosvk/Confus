#include "stdafx.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(BoxColliderTest)
{
	irr::IrrlichtDevice* m_Device = irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_NULL);

	TEST_METHOD(testBoxColliderSize)
	{
		ConfusShared::Physics::PhysicsWorld physicsWorld(m_Device);
		const float BoxSize = 9.f;
		auto node = m_Device->getSceneManager()->addCubeSceneNode(BoxSize);
		auto collider = physicsWorld.createBoxCollider(node);
		Assert::AreEqual(collider->getDimensions(), irr::core::vector3df(BoxSize));
	}

	TEST_METHOD(testBoxColliderScale)
	{
		ConfusShared::Physics::PhysicsWorld physicsWorld(m_Device);
		const float BoxSize = 3.3f;
		auto node = m_Device->getSceneManager()->addCubeSceneNode(1.0f, nullptr, -1, irr::core::vector3df(), irr::core::vector3df(),
			irr::core::vector3df(BoxSize));
		auto collider = physicsWorld.createBoxCollider(node);
		Assert::AreEqual(collider->getDimensions(), irr::core::vector3df(BoxSize));
	}
};