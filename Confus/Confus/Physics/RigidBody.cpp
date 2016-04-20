#include <Bullet/btBulletDynamicsCommon.h>

#include "RigidBody.h"

namespace Confus
{
	namespace Physics
	{
		RigidBody::RigidBody(std::unique_ptr<btRigidBody>&& a_RigidBody)
			: m_Body(std::move(a_RigidBody))
		{
		}
	}
}