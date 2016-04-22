#pragma once
#include <Bullet/btBulletCollisionCommon.h>
#include <memory>
#include <Irrlicht/irrlicht.h>

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
		enum class ERigidBodyType
		{
			Static,
			Dynamic,
			Kinematic
		};

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
			
			/// <summary> The mass of the Rigid Body </summary>
			/// <remarks> Mirrored to retain state of mass when body is changed to static/kinematic </remarks>
			float m_Mass = 1.0f;
			ERigidBodyType m_Type;

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

			/// <summary> Sets the mass of the Rigid Body </summary>
			/// <param name="a_Mass"> The new mass </param>
			void setMass(float a_Mass);

			/// <summary> Makes the RigidBody dynamic, causing it to be influenced by events in the Physics World </summary>
			void makeDynamic();

			/// <summary> Makes the RigidBody static, making it immovable in the Physics World </summary>
			/// <remarks> Set velocity and set mass can no longer be called as long as the object stays static </remarks>
			void makeStatic();
			
			/// <summary> Makes the RigidBody kinematic, causing it to be uninfluenced by events in the Physics World </summary>
			/// <remarks> Set mass can no longer be called as long as the object stays kinematic </remarks>
			void makeKinematic();
		private:
			/// <summary>
			/// Sets the position of the attached scene node in absolute/world terms
			/// </summary>
			/// <param name="a_Position"> The world postion </param>
			void setAbsolutePosition(irr::core::vector3df a_Position) const;
		};
	}
}