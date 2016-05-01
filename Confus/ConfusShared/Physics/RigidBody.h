#pragma once
#include <Bullet/btBulletCollisionCommon.h>
#include <memory>
#include <Irrlicht/irrlicht.h>

namespace Confus
{
	namespace Physics
	{
		/// <summary> The type of RigidBody, depicting how it will be influenced by the physics world </summary>
		enum class ERigidBodyType
		{
			/// <summary> The Rigid Body is not updated or influenced by the physics world </summary>
			Static,
			/// <summary> The Rigid Body is influenced by all effects in the physics world </summary>
			Dynamic,
			/// <summary> The Rigid Body is only influenced by manually adjusting its properties </summary>
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

			/// <summary> The type of Rigid Body </summary>
			ERigidBodyType m_Type;
			
			/// <summary> The offset from the scene node </summary>
			irr::core::vector3df m_Offset = irr::core::vector3df(0.0f);

			/// <summary> The motion state, used for moving kinematic bodies and interpolation </summary>
			std::unique_ptr<btDefaultMotionState> m_MotionState;

			/// <summary> Whether this Rigid Body is allowed to sleep when it is inactive for a longer period </summary>
			/// <remarks> Mirrored to retain the correct state when an object is re-activated </summary>
			bool m_SleepingAllowed = true;

			/// <summary> Whether this is a trigger Rigid Body, making it excluded from collision responses </summary>
			/// <remarks> Mirrored as being inactive uses the same flag to disallow collision responses </remarks>
			bool m_Trigger = false;

			/// <summary> Whether this Rigid Body is activated </summary>
			/// </remarks> Mirrored as being a trigger collider uses the same flag to disallow collision responses </remarks>
			bool m_Active = true;
		public:
			/// <summary> Initializes a new instance of the <see cref="RigidBody"/> class </summary>
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

			/// <summary> Makes the RigidBody dynamic, causing it to be influenced by events in the Physics World </summary>
			void makeDynamic();

			/// <summary> Makes the RigidBody static, making it immovable in the Physics World </summary>
			/// <remarks> Set velocity and set mass can no longer be called as long as the object stays static </remarks>
			void makeStatic();
			
			/// <summary> Makes the RigidBody kinematic, causing it to be uninfluenced by events in the Physics World </summary>
			/// <remarks> Set mass can no longer be called as long as the object stays kinematic </remarks>
			void makeKinematic();
			
			/// <summary> Makes this RigidBody a trigger collider, causing it to stop being influenced and stop influencing collision responses </summary>
			void enableTriggerState();

			/// <summary> Makes this RigidBody a regular collider, causing it to start being influenced and start influencing collision responses </summary>
			void disableTriggerState();
			
			/// <summary> Sets the velocity </summary>
			/// <param name="a_Velocity"> The new velocity </param>
			void setVelocity(irr::core::vector3df a_Velocity) const;
			
			/// <summary> Gets the linear velocity </summary>
			/// <returns> The linear velocity </returns>
			irr::core::vector3df getVelocity() const;
			
			/// <summary> Applies the given force to this Rigid Body</summary>
			/// <param name="a_Force">The force to apply </param>
			void applyForce(irr::core::vector3df a_Force) const;
			
			/// <summary> Deactivates the Rigid Body, excluding it from creating and receiving collision responses </summary>
			void deactivate();
			
			/// <summary> Activates the Rigid Body, causing it to, if it is not a trigger, create and receive collision responses </summary>
			void activate();
			
			/// <summary> Disables the ability for the physics engine to allow it to sleep after a certain time </summary>
			void disableSleeping();
			
			/// <summary> Determines whether the RIgid Body is activated </summary>
			/// <returns> Whether the Rigid Body is active </returns>
			bool isActive() const;
			
			/// <summary> Determines whether this instance is a trigger collider </summary>
			/// <returns> Whether this is a trigger collider </returns>
			bool isTrigger() const;

			irr::core::vector3df getOffset() const;
			void setOffset(irr::core::vector3df a_Offset);
		private:
			/// <summary> Sets the position of the attached scene node in absolute/world terms </summary>
			/// <param name="a_Position"> The world postion </param>
			void setAbsoluteTransform(const btTransform& a_Transform) const;

			/// <summary> Synchronizes the rigid body transform with the scene node's transform </summary>
			void syncRigidBodyTransform() const;

			/// <summary> Gets the euler angles. </summary>
			irr::core::vector3df toIrrlichtEuler(btQuaternion& a_Rotation) const;
						
			/// <summary> Extracts the transform of the Scene Node into a bullet transform</summary>
			/// <returns> The bullet transform </returns>
			btTransform extractTransform() const;
		};
	}
}