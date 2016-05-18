#pragma once
#include <Bullet/btBulletCollisionCommon.h>
#include <memory>
#include <functional>

#include "Collider.h"

namespace ConfusShared
{
	namespace Physics
	{
		class CollisionRegistrar;

		/// <summary> A collider in the shape of a Box to handle collisions </summary>
		/// <seealso cref="ICollider" />
		class BoxCollider : public Collider
		{
		private:
			/// <summary> The collision shape used for Bullet physics </summary>
			/// <remarks> 
			/// Allocated on the heap, as this is how the PhysicsWorld creates it, which in turn
			/// is necessary for Bullet to keep a pointer in the btRigidBody to the shape
			/// </remarks>
			std::unique_ptr<btBoxShape> m_Shape;

			/// <summary> The CollisionRegistrar that keeps track of the collisions occurred </summary>
			CollisionRegistrar& m_CollisionRegistrar;
		public:
			/// <summary>
			/// Initializes a new instance of the <see cref="BoxCollider" /> class
			/// </summary>
			/// <param name="a_Shape">The Bullet shape</param>
			/// <param name="a_RigidBody">The attached rigid body</param>
			/// <param name="a_CollisionObject">The collision object.</param>
			BoxCollider(std::unique_ptr<btBoxShape>&& a_Shape, RigidBody* a_RigidBody,
				CollisionRegistrar& a_CollisionRegistrar);
			
			/// <summary> Finalizes an instance of the <see cref="BoxCollider"/> class. </summary>
			virtual ~BoxCollider() override = default;
			
			/// <summary> Sets the trigger enter callback </summary>
			/// <param name="a_Callback">The callback to use once a Trigger Enter event has occurred </param>
			void setTriggerEnterCallback(std::function<void(BoxCollider* a_Other)>& a_Callback);

			/// <summary> Sets the trigger enter callback </summary>
			/// <param name="a_Callback">The callback to use once a Trigger Enter event has occurred </param>
			void setTriggerEnterCallback(std::function<void(BoxCollider* a_Other)>&& a_Callback);
		};
	}
}