#pragma once
#include <Bullet/btBulletCollisionCommon.h>
#include <memory>

#include "ICollider.h"

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
		class BoxCollider : public ICollider
		{
		private:
			std::unique_ptr<btTransform> m_Transform;
			std::unique_ptr<btBoxShape> m_Shape; 
			irr::scene::ISceneNode* m_AttachedNode;
		public:
			BoxCollider(std::unique_ptr<btBoxShape>&& a_Shape, irr::scene::ISceneNode* a_AttachedNode);
		};
	}
}