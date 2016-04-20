#pragma once
#include <Bullet/btBulletCollisionCommon.h>
#include <memory>

#include "ICollider.h"

namespace Confus
{
	namespace Physics
	{
		class BoxCollider : public ICollider
		{
		private:
			std::unique_ptr<btBoxShape> m_Shape; 
		public:
			BoxCollider(std::unique_ptr<btBoxShape>&& a_Shape);
		};
	}
}