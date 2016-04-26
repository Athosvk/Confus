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
			m_Mass(static_cast<btScalar>(1.0) / m_Body->getInvMass()),
			m_MotionState(std::make_unique<btDefaultMotionState>(extractTransform()))
		{
			m_Body->setMotionState(m_MotionState.get());
			m_Body->getInvMass() <= static_cast<btScalar>(0.00001) ? makeStatic() : makeDynamic();
			syncRigidBodyTransform();
		}

		void RigidBody::onPrePhysicsUpdate() const
		{
			syncRigidBodyTransform();
		}

		void RigidBody::onPostPhysicsUpdate() const
		{
			btTransform transform;
			m_MotionState->getWorldTransform(transform);
			setAbsoluteTransform(transform);
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
			m_Body->setActivationState(DISABLE_DEACTIVATION);
		}

		void RigidBody::makeStatic()
		{
			m_Type = ERigidBodyType::Static;
			m_Body->setCollisionFlags((m_Body->getCollisionFlags() |
				btRigidBody::CollisionFlags::CF_STATIC_OBJECT) &
				~btRigidBody::CollisionFlags::CF_KINEMATIC_OBJECT);
			m_Body->setMassProps(static_cast<btScalar>(0.0f), m_Body->getLocalInertia());
			m_Body->setActivationState(ACTIVE_TAG);
		}

		void RigidBody::makeKinematic()
		{
			m_Type = ERigidBodyType::Kinematic;
			m_Body->setCollisionFlags((m_Body->getCollisionFlags() | 
				btRigidBody::CollisionFlags::CF_KINEMATIC_OBJECT) &
				~btRigidBody::CollisionFlags::CF_STATIC_OBJECT);
			m_Body->setMassProps(static_cast<btScalar>(0.0f), m_Body->getLocalInertia());
			m_Body->setActivationState(DISABLE_DEACTIVATION);
		}

		void RigidBody::enableTriggerState()
		{
			m_Body->setCollisionFlags(m_Body->getCollisionFlags() | btRigidBody::CollisionFlags::CF_NO_CONTACT_RESPONSE);
		}

		void RigidBody::disableTriggerState()
		{
			m_Body->setCollisionFlags(m_Body->getCollisionFlags() & ~btRigidBody::CollisionFlags::CF_NO_CONTACT_RESPONSE);
		}

		void RigidBody::setVelocity(irr::core::vector3df a_Velocity) const
		{
			m_Body->setLinearVelocity(PhysicsWorld::toBulletVector(a_Velocity));
		}

		irr::core::vector3df RigidBody::getVelocity() const
		{
			return PhysicsWorld::toIrrlichtVector(m_Body->getLinearVelocity());
		}

		void RigidBody::setAbsoluteTransform(const btTransform& a_Transform) const
		{
			if(m_AttachedNode->getParent() != nullptr)
			{
				irr::core::matrix4 transformation = irr::core::IdentityMatrix;
				transformation.setTranslation(PhysicsWorld::toIrrlichtVector(a_Transform.getOrigin()));

				transformation.setRotationRadians(toIrrlichtEuler(a_Transform.getRotation()));
				irr::core::matrix4 inverseParentMatrix;
				m_AttachedNode->getParent()->getAbsoluteTransformation().getInverse(inverseParentMatrix);
				transformation = inverseParentMatrix * transformation;
				m_AttachedNode->setPosition(transformation.getTranslation());
			}
			else
			{
				m_AttachedNode->setPosition(PhysicsWorld::toIrrlichtVector(a_Transform.getOrigin()));
				m_AttachedNode->setRotation(toIrrlichtEuler(a_Transform.getRotation()) * irr::core::DEGTORAD);
			}
		}

		void RigidBody::syncRigidBodyTransform() const
		{
			m_MotionState->setWorldTransform(extractTransform());
		}

		irr::core::vector3df RigidBody::toIrrlichtEuler(btQuaternion& a_Rotation) const
		{
			irr::core::vector3df eulerAngles;
			irr::core::quaternion(a_Rotation.x(), a_Rotation.y(), a_Rotation.z(), a_Rotation.w()).toEuler(eulerAngles);
			return eulerAngles;
		}

		btTransform RigidBody::extractTransform() const
		{
			btTransform transform = btTransform::getIdentity();
			transform.setOrigin(PhysicsWorld::toBulletVector(m_AttachedNode->getAbsolutePosition()));
			auto eulerAngles = m_AttachedNode->getRotation() * irr::core::DEGTORAD;
			transform.setRotation(btQuaternion(eulerAngles.Y, eulerAngles.X, eulerAngles.Z));
			return transform;
		}
	}
}