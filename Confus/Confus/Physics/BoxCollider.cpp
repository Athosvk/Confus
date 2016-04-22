#include <Irrlicht/irrlicht.h>

#include "BoxCollider.h"
#include "RigidBody.h"

namespace Confus
{
	namespace Physics
	{
		BoxCollider::BoxCollider(std::unique_ptr<btBoxShape>&& a_Shape, RigidBody* a_RigidBody)
			: ICollider(a_RigidBody), 
			m_Shape(std::move(a_Shape))
		{
		}
	}
}