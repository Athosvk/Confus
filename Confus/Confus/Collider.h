#pragma once
#include <Irrlicht/irrlicht.h>
#include <functional>

namespace Confus
{
    class Collider : public irr::scene::ICollisionCallback
    {
    private:
        std::function<bool(irr::scene::ISceneNode* a_CollidedNode)> m_Callback;

    public:
        Collider(irr::scene::ISceneNodeAnimatorCollisionResponse* a_AttachedNode);
        
        virtual bool onCollision(const irr::scene::ISceneNodeAnimatorCollisionResponse& a_Animator) override;
        void setCallback(std::function<bool(irr::scene::ISceneNode* a_CollidedNode)>& a_Callback);
    };
}
