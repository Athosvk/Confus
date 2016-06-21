#pragma once
#include <Bullet/btBulletCollisionCommon.h>
#include <memory>
#include <functional>
#include <Irrlicht/irrlicht.h>

#include "Collider.h"

namespace ConfusShared
{
	namespace Physics
	{
		class PhysicsWorld;
		class CollisionRegistrar;

		/// <summary> A collider in the shape of a Box to handle collisions </summary>
		/// <seealso cref="ICollider" />
		class BoxCollider : public Collider
		{
		private:
			/// <summary> The collision shape used for Bullet physics </summary>
			/// <remarks> 
			/// Allocated on the heap, as this is how the PhysicsWorld creates it and requires 
			/// 16-byte alignment, which in turn
			/// is necessary for Bullet to keep a pointer in the btRigidBody to the shape
			/// </remarks>
			std::unique_ptr<btBoxShape> m_Shape;

			/// <summary> 
			/// The CollisionRegistrar that keeps track of the collisions occurred 
			/// that occurred in the prevous physics simulation and the current
			/// simulation step, so that we can distribute callbacks  for such events
			/// </summary>
			CollisionRegistrar& m_CollisionRegistrar;
		public:
			/// <summary>Initializes a new instance of the <see cref="BoxCollider" /> class</summary>
			/// <param name="a_Shape">The Bullet shape</param>
			/// <param name="a_RigidBody">The attached rigid body</param>
			/// <param name="a_CollisionRegistrar">The collision registrar for registering collisions</param>
			/// <param name="a_PhysicsWorld">Reference to the physics world.</param>
			BoxCollider(std::unique_ptr<btBoxShape>&& a_Shape, RigidBody* a_RigidBody,
				CollisionRegistrar& a_CollisionRegistrar, PhysicsWorld* a_PhysicsWorld);
			
			/// <summary> Finalizes an instance of the <see cref="BoxCollider"/> class. </summary>
			virtual ~BoxCollider() override = default;
			
			/// <summary> Sets the trigger enter callback </summary>
			/// <param name="a_Callback">The callback to use once a Trigger Enter event has occurred </param>
			void setTriggerEnterCallback(const std::function<void(BoxCollider* a_Other)>& a_Callback);
			
			/// <summary>Gets the dimensions of the box collider</summary>
			/// <returns>The dimensions</returns>
			irr::core::vector3df getDimensions() const;
		};
	}
}