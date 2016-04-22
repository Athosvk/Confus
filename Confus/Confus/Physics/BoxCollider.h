#pragma once
#include <Bullet/btBulletCollisionCommon.h>
#include <memory>

#include "ICollider.h"

namespace Confus
{
	namespace Physics
	{
		/// <summary> A collider in the shape of a Box to handle collisions </summary>
		/// <seealso cref="ICollider" />
		class BoxCollider : public ICollider
		{
		private:
			/// <summary> The collision shape used for Bullet physics </summary>
			/// <remarks> 
			/// Allocated on the heap, as this is how the PhysicsWorld creates it, which in turn
			/// is necessary for Bullet to keep a pointer in the btRigidBody to the shape
			/// </remarks>
			std::unique_ptr<btBoxShape> m_Shape;
		public:
			/// <summary> Initializes a new instance of the <see cref="BoxCollider"/> class </summary>
			/// <param name="a_Shape"> The Bullet shape </param>
			BoxCollider(std::unique_ptr<btBoxShape>&& a_Shape);
		};
	}
}