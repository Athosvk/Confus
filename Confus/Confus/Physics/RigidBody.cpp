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
			m_AttachedNode(a_AttachedNode)
		{
		}

		void RigidBody::onPrePhysicsUpdate()
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

		void RigidBody::synchronizeTransform()
		{
			auto transform = m_Body->getWorldTransform();
			setAbsoluteTransformation(PhysicsWorld::toIrrlichtVector(transform.getOrigin()));
		}

		irr::scene::ISceneNode* RigidBody::getAttachedNode() const
		{
			return m_AttachedNode;
		}

		void RigidBody::setAbsoluteTransformation(irr::core::vector3df a_Position)
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