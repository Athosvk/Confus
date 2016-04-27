#include "PhysicsWorld.h"
#include "BoxCollider.h"

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


		PhysicsWorld::PhysicsWorld(irr::IrrlichtDevice* a_Device)
			: m_DebugDrawer(a_Device, &m_World)
		{
			m_World.setDebugDrawer(&m_DebugDrawer);
			m_DebugDrawer.setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawContactPoints
			| btIDebugDraw::DBG_DrawText);
			m_World.setGravity(btVector3(0.0f, -9.81f, 0.0f));
		}

		PhysicsWorld::~PhysicsWorld()
		{
			auto& collisionObjects = m_World.getCollisionObjectArray();
			for(int i = m_World.getNumCollisionObjects() - 1; i >= 0; --i)
			{
				m_World.removeCollisionObject(collisionObjects[i]);
			}
		}

		void PhysicsWorld::stepSimulation(float a_DeltaTime)
		{
			prePhysicsUpdate();
			m_World.stepSimulation(a_DeltaTime);
			postPhysicsUpdate();
		}

		void PhysicsWorld::drawDebugInformation()
		{
			m_DebugDrawer.draw();
		}

		void PhysicsWorld::prePhysicsUpdate() const
		{
			for(auto& colliderPair : m_Colliders)
			{
				colliderPair.Body->onPrePhysicsUpdate();
			}
		}

		void PhysicsWorld::postPhysicsUpdate() const
		{
			for(auto& colliderPair : m_Colliders)
			{
				colliderPair.Body->onPostPhysicsUpdate();
			}
		}


		BoxCollider* PhysicsWorld::createBoxCollider(irr::core::vector3df a_Extents, irr::scene::ISceneNode* a_AttachedNode, 
			ECollisionFilter a_Group, ECollisionFilter a_Mask)
		{
			auto shape = std::make_unique<btBoxShape>(toBulletVector(a_Extents / 2));
			auto rigidBody = createRigidBody(shape.get(), a_AttachedNode, a_Group, a_Mask);
			auto collider = std::make_unique<BoxCollider>(std::move(shape), rigidBody.get());
			m_Colliders.emplace_back(std::move(collider), std::move(rigidBody));
			return static_cast<BoxCollider*>(m_Colliders.back().Shape.get());
		}

		BoxCollider* PhysicsWorld::createBoxCollider(irr::scene::ISceneNode* a_AttachedNode, ECollisionFilter a_Group,
			ECollisionFilter a_Mask)
		{
			return createBoxCollider(a_AttachedNode->getBoundingBox().getExtent() * a_AttachedNode->getScale(), 
				a_AttachedNode, a_Group, a_Mask);
		}

		btVector3 PhysicsWorld::toBulletVector(const irr::core::vector3df& a_Vector)
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
			irr::scene::ISceneNode* a_AttachedNode, ECollisionFilter a_Group, ECollisionFilter a_Mask)
		{
			btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo =
				btRigidBody::btRigidBodyConstructionInfo(1.0f, nullptr, a_Shape);
			auto rigidBody = std::make_unique<btRigidBody>(rigidBodyInfo);
			m_World.addRigidBody(rigidBody.get(), static_cast<short>(a_Group), static_cast<short>(a_Mask));
			return std::make_unique<RigidBody>(std::move(rigidBody), a_AttachedNode);
		}
	}
}