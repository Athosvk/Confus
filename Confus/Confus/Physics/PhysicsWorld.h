#pragma once
#include <Irrlicht/irrlicht.h>
#include <Bullet/btBulletDynamicsCommon.h>
#include <vector>
#include <memory>

#include "ICollider.h"
#include "RigidBody.h"

namespace Confus
{
	namespace Physics
	{
		class PhysicsWorld
		{
		private:
			struct ColliderPair
			{
			public:
				std::unique_ptr<ICollider> Shape;
				std::unique_ptr<RigidBody> Body;

				ColliderPair(std::unique_ptr<ICollider>&& a_Shape,
					std::unique_ptr<RigidBody>&& a_RigidBody);
			};

			btCollisionConfiguration* m_Configuration = new btDefaultCollisionConfiguration();
			btCollisionDispatcher* m_Dispatcher = new btCollisionDispatcher(m_Configuration);
			btBroadphaseInterface* m_BroadPhaseInterface = new btDbvtBroadphase();
			btSequentialImpulseConstraintSolver* m_Solver = new btSequentialImpulseConstraintSolver;

			btDiscreteDynamicsWorld* m_World = new btDiscreteDynamicsWorld(m_Dispatcher,
				m_BroadPhaseInterface, m_Solver, m_Configuration);
			std::vector<ColliderPair> m_Colliders;
		public:
			PhysicsWorld();

			void createBoxCollider(irr::core::vector3df a_Extents, 
				irr::scene::ISceneNode* a_AttachedNode);
			void physicsUpdate(float a_DeltaTime);
			static btVector3 toBulletVector(const irr::core::vector3d<float>& a_Vector);
			static irr::core::vector3df toIrrlichtVector(const btVector3& a_Vector);
		private:
			void prePhysicsUpdate();
			void synchronizeTransforms();
			std::unique_ptr<RigidBody> createRigidBody(btCollisionShape* a_Shape,
				irr::scene::ISceneNode* a_AttachedNode) const;
		};
	}
}


