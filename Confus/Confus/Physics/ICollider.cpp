#include "ICollider.h"
#include "RigidBody.h"

namespace Confus
{
	namespace Physics
	{
		ICollider::ICollider(RigidBody* a_RigidBody)
			: m_RigidBody(a_RigidBody)
		{
		}
		
		RigidBody* ICollider::getRigidBody()
		{
			return m_RigidBody;
		}
	}
}