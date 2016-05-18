#include <Irrlicht/irrlicht.h>

#include "BoxCollider.h"
#include "RigidBody.h" 
#include "CollisionRegistrar.h"
#include "PhysicsWorld.h"

namespace Confus
{
	namespace Physics
	{
        BoxCollider::BoxCollider(std::unique_ptr<btBoxShape>&& a_Shape, RigidBody* a_RigidBody,
            CollisionRegistrar& a_CollisionRegistrar, PhysicsWorld* a_PhysicsWorld)
            : Collider(a_RigidBody),
            m_Shape(std::move(a_Shape)),
            m_CollisionRegistrar(a_CollisionRegistrar),
            m_PhysicsWorld(a_PhysicsWorld)
		{
		}

		void BoxCollider::setTriggerEnterCallback(std::function<void(BoxCollider*a_Other)>& a_Callback)
		{
			m_CollisionRegistrar.setTriggerEnterCallback(this, a_Callback);
		}

		void BoxCollider::setTriggerEnterCallback(std::function<void(BoxCollider*a_Other)>&& a_Callback)
		{
			m_CollisionRegistrar.setTriggerEnterCallback(this, a_Callback);
		}

        void BoxCollider::deleteBoxCollider()
        {
            m_PhysicsWorld->removeCollider(this);
        }
	}
}