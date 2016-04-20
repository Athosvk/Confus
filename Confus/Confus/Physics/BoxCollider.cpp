#include <Irrlicht/irrlicht.h>

#include "BoxCollider.h"

namespace Confus
{
	namespace Physics
	{
		BoxCollider::BoxCollider(std::unique_ptr<btBoxShape>&& a_Shape)
			: m_Shape(std::move(a_Shape))
		{
		}
	}
}