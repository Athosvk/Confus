#include "stdafx.h"

#include <Confus/Collider.h>

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ConfusTest
{
	TEST_CLASS(ColliderTest)
	{
		irr::IrrlichtDevice* m_Device = irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_NULL);

		TEST_METHOD(testCallback)
		{
			auto animator = createDummyAnimator();
			Confus::Collider collider(animator);
			bool triggered = false;
			collider.setCallback([&triggered](irr::scene::ISceneNode*)->bool
			{
				triggered = true;
				return true;
			});
			collider.onCollision(*animator);
			Assert::IsTrue(triggered);
		}

		TEST_METHOD(testReturnParameter)
		{
			auto animator = createDummyAnimator();
			Confus::Collider collider(animator);

			collider.setCallback([](irr::scene::ISceneNode* a_CollidedNode)->bool
			{
				return false;
			});
			Assert::IsFalse(collider.onCollision(*animator));
		}
		
		irr::scene::ISceneNodeAnimatorCollisionResponse* createDummyAnimator()
		{
			auto sceneNode = m_Device->getSceneManager()->addEmptySceneNode();
			auto animator = m_Device->getSceneManager()->
				createCollisionResponseAnimator(nullptr, sceneNode);
			return animator;
		}
	};
}