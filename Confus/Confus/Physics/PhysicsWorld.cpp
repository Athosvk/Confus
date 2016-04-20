#include "PhysicsWorld.h"
#include "BoxCollider.h"
#include "../Game.h"

namespace Confus
{
	namespace Physics
	{
		PhysicsWorld::ColliderPair::ColliderPair(std::unique_ptr<ICollider>&& a_Shape,
			std::unique_ptr<RigidBody>&& a_RigidBody)
			: Shape(std::move(a_Shape)),
			Body(std::move(a_RigidBody))
		{
		}

		PhysicsWorld::PhysicsWorld()
		{
		}

		void PhysicsWorld::createBoxCollider(irr::core::vector3df a_Extents,
			irr::scene::ISceneNode* a_AttachedNode)
		{
			auto shape = std::make_unique<btBoxShape>(toBulletVector(a_Extents / 2));
			auto rigidBody = createRigidBody(shape.get());
			auto collider = std::make_unique<BoxCollider>(std::move(shape), a_AttachedNode);
			m_Colliders.emplace_back(std::move(collider), std::move(rigidBody));
		}

		std::unique_ptr<RigidBody> PhysicsWorld::createRigidBody(btCollisionShape* a_Shape) const
		{
			btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo =
				btRigidBody::btRigidBodyConstructionInfo(0.0f, nullptr, a_Shape);
			auto rigidBody = std::make_unique<btRigidBody>(rigidBodyInfo);
			m_World->addRigidBody(rigidBody.get());
			return std::make_unique<RigidBody>(std::move(rigidBody));
		}
	}
}