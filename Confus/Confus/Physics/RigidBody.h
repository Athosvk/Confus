#pragma once
#include <Bullet/btBulletCollisionCommon.h>
#include <memory>

namespace Confus
{
	namespace Physics
	{
		class RigidBody
		{
		private:
			std::unique_ptr<btRigidBody> m_Body;

		public:
			RigidBody(std::unique_ptr<btRigidBody>&& a_RigidBody);
		};
	}
}