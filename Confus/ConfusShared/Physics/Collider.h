#pragma once
#include "PhysicsWorld.h"

namespace ConfusShared
{
	namespace Physics
	{
		class RigidBody;

		/// <summary> A generic interface that all colliders in the physics world need to implement </summary>
		class Collider
		{
		private:			
			/// <summary> The attached Rigid Body </summary>
			RigidBody* m_RigidBody;
			/// <summary> The attached PhysicsWorld </summary>
			PhysicsWorld* m_PhysicsWorld;
		protected:
			/// <summary>
			/// Initializes a new instance of the <see cref="Collider" /> class.
			/// </summary>
			/// <param name="a_RigidBody">The a_ rigid body.</param>
			/// <param name="a_PhysicsWorld">The a_ physics world.</param>
			Collider(RigidBody* a_RigidBody, PhysicsWorld* a_PhysicsWorld);
		public:			
			/// <summary> Finalizes an instance of the <see cref="Collider"/> class </summary>
			virtual ~Collider() = default;

			/// <summary> Gets the attached Rigid Body </summary>
			/// <returns> The attached Rigid Body </returns>
			RigidBody* getRigidBody();

			/// <summary> Deletes the collider and removes it from the world. </summary>
			void deleteCollider();
		};
	}
}
