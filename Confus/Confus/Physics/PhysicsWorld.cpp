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
			m_World->setGravity(btVector3(0.0f, -9.81f, 0.0f));
		}

		void PhysicsWorld::physicsUpdate(float a_DeltaTime) const
		{
			prePhysicsUpdate();
			m_World->stepSimulation(a_DeltaTime);
			synchronizeTransforms();
		}

		void PhysicsWorld::prePhysicsUpdate() const
		{
			for(auto& colliderPair : m_Colliders)
			{
				colliderPair.Body->onPrePhysicsUpdate();
			}
		}

		void PhysicsWorld::synchronizeTransforms() const
		{
			for(auto& colliderPair : m_Colliders)
			{
				colliderPair.Body->synchronizeTransform();
			}
		}

		void PhysicsWorld::createBoxCollider(irr::core::vector3df a_Extents,
			irr::scene::ISceneNode* a_AttachedNode)
		{
			auto shape = std::make_unique<btBoxShape>(toBulletVector(a_Extents / 2));
			auto rigidBody = createRigidBody(shape.get(), a_AttachedNode);
			auto collider = std::make_unique<BoxCollider>(std::move(shape));
			m_Colliders.emplace_back(std::move(collider), std::move(rigidBody));
		}

		btVector3 PhysicsWorld::toBulletVector(const irr::core::vector3d<float>& a_Vector)
		{
			return btVector3(static_cast<btScalar>(a_Vector.X),
				static_cast<btScalar>(a_Vector.Y),
				static_cast<btScalar>(a_Vector.Z));
		}

		irr::core::vector3df PhysicsWorld::toIrrlichtVector(const btVector3& a_Vector)
		{
			return irr::core::vector3df(static_cast<float>(a_Vector.x()),
				static_cast<float>(a_Vector.y()),
				static_cast<float>(a_Vector.z()));
		}

		std::unique_ptr<RigidBody> PhysicsWorld::createRigidBody(btCollisionShape* a_Shape, 
			irr::scene::ISceneNode* a_AttachedNode) const
		{
			btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo =
				btRigidBody::btRigidBodyConstructionInfo(0.1f, nullptr, a_Shape);
			auto rigidBody = std::make_unique<btRigidBody>(rigidBodyInfo);
			m_World->addRigidBody(rigidBody.get());
			return std::make_unique<RigidBody>(std::move(rigidBody), a_AttachedNode);
		}
	}
}