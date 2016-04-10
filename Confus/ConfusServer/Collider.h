#pragma once
#include <Irrlicht/irrlicht.h>
#include <functional>

namespace ConfusServer
{    
    /// <summary>
    /// Registers a callback for when a collision triggers on the given animated collision response
    /// </summary>
    /// <seealso cref="irr::scene::ICollisionCallback" />
    class Collider : public irr::scene::ICollisionCallback
    {
    private:        
        /// <summary>
        /// The callback called once a collision occured
        /// </summary>
        /// <returns> Whether the function has completed the handling of the collision </returns>
        std::function<bool(irr::scene::ISceneNode* a_CollidedNode)> m_Callback;

    public:        
        /// <summary>
        /// Initializes a new instance of the <see cref="Collider"/> class.
        /// </summary>
        /// <param name="a_AttachedNode">The node (animator) this collider should be based on.</param>
        Collider(irr::scene::ISceneNodeAnimatorCollisionResponse* a_AttachedNode);
                
        /// <summary>
        /// Called from Irrlicht once a collision has occured with this animator
        /// </summary>
        /// <param name="a_Animator">The response animator that registered the collision.</param>
        /// <returns>Whether the function has completed the handling of the collision.</returns>
        virtual bool onCollision(const irr::scene::ISceneNodeAnimatorCollisionResponse& a_Animator) override;        
        
        /// <summary>
        /// Sets the callback called when a collision triggers.
        /// </summary>
        /// <param name="a_Callback">The callback to use.</param>
        void setCallback(const std::function<bool(irr::scene::ISceneNode* a_CollidedNode)>& a_Callback);
    };
}
