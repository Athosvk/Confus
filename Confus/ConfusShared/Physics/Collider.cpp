#include "Collider.h"
#include "RigidBody.h"

namespace Confus
{
	namespace Physics
	{
		Collider::Collider(RigidBody* a_RigidBody)
			: m_RigidBody(a_RigidBody)
		{
		}
		
		RigidBody* Collider::getRigidBody()
		{
			return m_RigidBody;
		}
	}
}