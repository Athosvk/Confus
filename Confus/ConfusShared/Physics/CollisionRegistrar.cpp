#include "CollisionRegistrar.h"
#include "BoxCollider.h"
#include "RigidBody.h"

namespace ConfusShared
{
	namespace Physics
	{
		CollisionRegistrar::CollisionInfo::CollisionInfo(BoxCollider* a_ColliderA, 
			BoxCollider* a_ColliderB)
			: ColliderA(a_ColliderA),
			ColliderB(a_ColliderB)
		{
		}

		bool CollisionRegistrar::CollisionInfo::operator==(const CollisionInfo& a_Other)
		{
			return ColliderA == a_Other.ColliderA && ColliderB == a_Other.ColliderB;
		}

		CollisionRegistrar::CollisionRegistrar(btCollisionWorld* a_World)
			: m_CollisionWorld(a_World)
		{
		}

		void CollisionRegistrar::setTriggerEnterCallback(BoxCollider* a_Collider,
			std::function<void(BoxCollider* a_Other)>& a_Callback)
		{
			m_TriggerEnterCallbacks.emplace(a_Collider, a_Callback);
		}

		void CollisionRegistrar::setTriggerEnterCallback(BoxCollider* a_Collider,
			std::function<void(BoxCollider* a_Other)>&& a_Callback)
		{
			m_TriggerEnterCallbacks.emplace(a_Collider, std::move(a_Callback));
		}

		void CollisionRegistrar::onPostPhysicsUpdate()
		{
			for(int i = 0; i < m_CollisionWorld->getDispatcher()->getNumManifolds(); ++i)
			{
				auto manifold = m_CollisionWorld->getDispatcher()->getManifoldByIndexInternal(i);

				if(manifold->getNumContacts() > 0)
				{
					auto colliderA = static_cast<BoxCollider*>(manifold->getBody0()->getUserPointer());
					auto colliderB = static_cast<BoxCollider*>(manifold->getBody1()->getUserPointer());

					handleCollision(colliderA, colliderB);
				}
			}
			m_PreviousCollisions = std::move(m_Collisions);
		}

		void CollisionRegistrar::handleCollision(BoxCollider* a_ColliderA, BoxCollider* a_ColliderB)
		{
			m_Collisions.emplace_back(a_ColliderA, a_ColliderB);
			if(a_ColliderA->getRigidBody()->isActive() && a_ColliderB->getRigidBody()->isActive() &&
				(a_ColliderA->getRigidBody()->isTrigger() || a_ColliderB->getRigidBody()->isTrigger()) &&
				std::find(m_PreviousCollisions.begin(), m_PreviousCollisions.end(),
				CollisionInfo(a_ColliderA, a_ColliderB)) == m_PreviousCollisions.end())
			{
				if(m_TriggerEnterCallbacks.find(a_ColliderA) != m_TriggerEnterCallbacks.end())
				{
					m_TriggerEnterCallbacks[a_ColliderA](a_ColliderB);
				}
				if(m_TriggerEnterCallbacks.find(a_ColliderB) != m_TriggerEnterCallbacks.end())
				{
					m_TriggerEnterCallbacks[a_ColliderB](a_ColliderA);
				}
			}
		}
	}
}