#include "stdafx.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(RigidBodyTest)
{
	irr::IrrlichtDevice* m_Device = irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_NULL);

	TEST_METHOD(testTriggerState)
	{
		ConfusShared::Physics::PhysicsWorld physicsWorld(m_Device);
		auto collider = physicsWorld.createBoxCollider(m_Device->getSceneManager()->addEmptySceneNode());
		collider->getRigidBody()->enableTriggerState();
		Assert::IsTrue(collider->getRigidBody()->isTrigger());
	}

	TEST_METHOD(testDeactivation)
	{
		ConfusShared::Physics::PhysicsWorld physicsWorld(m_Device);
		auto collider = physicsWorld.createBoxCollider(m_Device->getSceneManager()->addEmptySceneNode());
		collider->getRigidBody()->deactivate();
		Assert::IsTrue(!collider->getRigidBody()->isActive());
	}
};