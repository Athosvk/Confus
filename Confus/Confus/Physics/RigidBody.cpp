#include <Bullet/btBulletDynamicsCommon.h>
#include <Irrlicht/irrlicht.h>

#include "RigidBody.h"
#include "PhysicsWorld.h"

namespace Confus
{
	namespace Physics
	{
		RigidBody::RigidBody(std::unique_ptr<btRigidBody>&& a_RigidBody, irr::scene::ISceneNode* a_AttachedNode)
			: m_Body(std::move(a_RigidBody)),
			m_AttachedNode(a_AttachedNode),
			m_Mass(static_cast<btScalar>(1.0) / m_Body->getInvMass())
		{
			m_Type = m_Body->getInvMass() == static_cast<btScalar>(0.0) ? ERigidBodyType::Static :
				ERigidBodyType::Dynamic;
		}

		void RigidBody::onPrePhysicsUpdate() const
		{
			btTransform transform = btTransform::getIdentity();
			transform.setOrigin(PhysicsWorld::toBulletVector(m_AttachedNode->getAbsolutePosition()));
			btQuaternion rotation;
			auto eulerAngles = PhysicsWorld::toBulletVector(m_AttachedNode->getRotation());
			//Y, X, Z matches yaw, pitch, roll
			rotation.setEuler(eulerAngles.y(), eulerAngles.x(), eulerAngles.z());
			transform.setRotation(rotation);
			m_Body->setWorldTransform(transform);
		}

		void RigidBody::onPostPhysicsUpdate() const
		{
			auto transform = m_Body->getWorldTransform();
			setAbsolutePosition(PhysicsWorld::toIrrlichtVector(transform.getOrigin()));
		}

		irr::scene::ISceneNode* RigidBody::getAttachedNode() const
		{
			return m_AttachedNode;
		}

		void RigidBody::makeDynamic()
		{
			m_Type = ERigidBodyType::Dynamic;
			m_Body->setCollisionFlags(m_Body->getCollisionFlags() &
				~btRigidBody::CollisionFlags::CF_STATIC_OBJECT &
				~btRigidBody::CollisionFlags::CF_KINEMATIC_OBJECT);
			m_Body->setMassProps(static_cast<btScalar>(m_Mass), m_Body->getLocalInertia());
		}

		void RigidBody::makeStatic()
		{
			m_Type = ERigidBodyType::Static;
			m_Body->setCollisionFlags((m_Body->getCollisionFlags() |
				btRigidBody::CollisionFlags::CF_STATIC_OBJECT) &
				~btRigidBody::CollisionFlags::CF_KINEMATIC_OBJECT);
			m_Body->setMassProps(static_cast<btScalar>(0.0f), m_Body->getLocalInertia());
		}

		void RigidBody::makeKinematic()
		{
			m_Type = ERigidBodyType::Kinematic;
			m_Body->setCollisionFlags((m_Body->getCollisionFlags() | 
				btRigidBody::CollisionFlags::CF_KINEMATIC_OBJECT) &
				~btRigidBody::CollisionFlags::CF_STATIC_OBJECT);
			m_Body->setMassProps(static_cast<btScalar>(0.0f), m_Body->getLocalInertia());
		}

		void RigidBody::enableTriggerState()
		{
			m_Trigger = true;
			m_Body->setCollisionFlags(m_Body->getCollisionFlags() | btRigidBody::CollisionFlags::CF_NO_CONTACT_RESPONSE);
		}

		void RigidBody::disableTriggerState()
		{
			m_Trigger = true;
			m_Body->setCollisionFlags(m_Body->getCollisionFlags() & ~btRigidBody::CollisionFlags::CF_NO_CONTACT_RESPONSE);
		}

		void RigidBody::setAbsolutePosition(irr::core::vector3df a_Position) const
		{
			if(m_AttachedNode->getParent() != nullptr)
			{
				irr::core::matrix4 transformation = irr::core::IdentityMatrix;
				transformation.setTranslation(a_Position);
				transformation *= m_AttachedNode->getParent()->getAbsoluteTransformation();
				m_AttachedNode->setPosition(transformation.getTranslation());
			}
			else
			{
				m_AttachedNode->setPosition(a_Position);
			}
		}
	}
}