#include "BoxCollider.h"

namespace Confus
{
	namespace Physics
	{
		BoxCollider::BoxCollider(std::unique_ptr<btBoxShape>&& a_Shape, 
			irr::scene::ISceneNode* a_AttachedNode)
			: m_Shape(std::move(a_Shape)),
			m_AttachedNode(a_AttachedNode)
		{
		}
	}
}