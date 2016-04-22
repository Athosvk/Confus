#pragma once
#include <Bullet/btBulletCollisionCommon.h>
#include <memory>

namespace irr
{
	namespace scene
	{
		class ISceneNode;
	}
}

namespace Confus
{
	namespace Physics
	{
		class RigidBody
		{
		private:
			std::unique_ptr<btRigidBody> m_Body;
			irr::scene::ISceneNode* m_AttachedNode;

		public:
			RigidBody(std::unique_ptr<btRigidBody>&& a_RigidBody, irr::scene::ISceneNode* a_AttachedNode);

			void onPrePhysicsUpdate() const;
			void synchronizeTransform() const;
			irr::scene::ISceneNode* getAttachedNode() const;
		private:
			void setAbsoluteTransformation(irr::core::vector3df a_Position) const;
		};
	}
}