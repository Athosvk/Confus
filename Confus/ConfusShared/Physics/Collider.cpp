#include "Collider.h"
#include "RigidBody.h"
#include "PhysicsWorld.h"

namespace ConfusShared
{
	namespace Physics
	{

		Collider::Collider(RigidBody* a_RigidBody, PhysicsWorld* a_PhysicsWorld)
			: m_RigidBody(a_RigidBody),
			m_PhysicsWorld(a_PhysicsWorld)
		{
		}

		void Collider::deleteCollider()
		{
			m_PhysicsWorld->removeCollider(this);
		}
		
		RigidBody* Collider::getRigidBody()
		{
			return m_RigidBody;
		}
	}
}
