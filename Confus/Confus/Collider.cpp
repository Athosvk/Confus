#include "Collider.h"

namespace Confus
{
    Collider::Collider(irr::scene::ISceneNodeAnimatorCollisionResponse* a_AttachedNode)
    {
        a_AttachedNode->setCollisionCallback(this);
    }

    bool Collider::onCollision(const irr::scene::ISceneNodeAnimatorCollisionResponse& a_Animator)
    {
        return m_Callback(a_Animator.getCollisionNode());
    }

    void Collider::setCallback(std::function<bool(irr::scene::ISceneNode*a_CollidedNode)>& a_Callback)
    {
        m_Callback = a_Callback;
    }
}