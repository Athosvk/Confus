#pragma once
#include <Bullet/btBulletCollisionCommon.h>
#include <memory>

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
		/// <summary> Represents the RigidBody in the Physics World, associated with some collision shape </summary>
		class RigidBody
		{
		private:
			/// <summary> The RigidBody used for Bullet physics </summary>
			/// <remarks> 
			/// Allocated on the heap, as this is how the PhysicsWorld creates it, which in turn
			/// is necessary for Bullet
			/// </remarks>
			std::unique_ptr<btRigidBody> m_Body;

			/// <summary> The SceneNode with which this RigidBody is associated </summary>
			irr::scene::ISceneNode* m_AttachedNode;

		public:
			/// <summary>
			/// Initializes a new instance of the <see cref="RigidBody"/> class.
			/// </summary>
			/// <param name="a_RigidBody">The Bullet rigid body </param>
			/// <param name="a_AttachedNode">The attached/associated scene node </param>
			RigidBody(std::unique_ptr<btRigidBody>&& a_RigidBody, irr::scene::ISceneNode* a_AttachedNode);

			/// <summary> Updates the physics world simulation structure before the physics update commences </summary>
			void onPrePhysicsUpdate() const;

			/// <summary> Syncs the transform, primarily position, after the physics update </summary>
			void onPostPhysicsUpdate() const;

			/// <summary> Gets the associated/attached scene node </summary>
			/// <returns> The associated/attached scene node </returns>
			irr::scene::ISceneNode* getAttachedNode() const;
		private:
			/// <summary>
			/// Sets the position of the attached scene node in absolute/world terms
			/// </summary>
			/// <param name="a_Position"> The world postion </param>
			void setAbsolutePosition(irr::core::vector3df a_Position) const;
		};
	}
}