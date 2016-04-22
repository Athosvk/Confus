#pragma once
#include <Irrlicht/irrlicht.h>
#include <Bullet/btBulletDynamicsCommon.h>
#include <vector>
#include <memory>

#include "ICollider.h"
#include "RigidBody.h"

namespace Confus
{
	namespace Physics
	{
		/// <summary>
		/// The world in which all, rigid body, physics simulations are performed by the physics engine
		/// </summary>
		class PhysicsWorld
		{
		private:
			/// <summary> A combination of a RigidBody and a collider </summary>
			struct ColliderPair
			{
			public:				
				/// <summary> The (collider)shape of this collision structure </summary>
				std::unique_ptr<ICollider> Shape;				
				/// <summary> The rigid body of this collision structure </summary>
				std::unique_ptr<RigidBody> Body;

				/// <summary> Initializes a new instance of the <see cref="ColliderPair"/> struct </summary>
				/// <param name="a_Shape">The (collider)shape </param>
				/// <param name="a_RigidBody">The rigid body </param>
				ColliderPair(std::unique_ptr<ICollider>&& a_Shape,
					std::unique_ptr<RigidBody>&& a_RigidBody);
			};

			/// <summary> The configuration settings for the physics world </summary>
			btCollisionConfiguration* m_Configuration = new btDefaultCollisionConfiguration();			
			/// <summary> The collision dispatcher used by the physics engine</summary>
			btCollisionDispatcher* m_Dispatcher = new btCollisionDispatcher(m_Configuration);			
			/// <summary> The broadphase collision handler </summary>
			btBroadphaseInterface* m_BroadPhaseInterface = new btDbvtBroadphase();			
			/// <summary> The constraint solver for the physics world </summary>
			btSequentialImpulseConstraintSolver* m_Solver = new btSequentialImpulseConstraintSolver;
			/// <summary> The internal, rigid body, simulation world of the physics engine </summary>
			btDiscreteDynamicsWorld* m_World = new btDiscreteDynamicsWorld(m_Dispatcher, m_BroadPhaseInterface, 
				m_Solver, m_Configuration);			
			/// <summary> The collider - rigid body pairs that are currently instantiated in the physics world </summary>
			std::vector<ColliderPair> m_Colliders;
		public:
			/// <summary> Initializes a new instance of the <see cref="PhysicsWorld"/> class </summary>
			PhysicsWorld();

			/// <summary> Creates and places a new box collider into the physics world for simulations </summary>
			/// <param name="a_Extents">The extents/dimensions of the collider to be created </param>
			/// <param name="a_AttachedNode">The node the collider is attached to </param>
			void createBoxCollider(irr::core::vector3df a_Extents, irr::scene::ISceneNode* a_AttachedNode);

			/// <summary> Advanced the physics simulation based on the entered delta time value </summary>
			/// <param name="a_DeltaTime"> 
			/// The time with which to advance the physics simulation usually equal to the interval between two 
			/// fixed/physics updates
			/// </param>
			void stepSimulation(float a_DeltaTime) const;

			/// <summary> Converts the given Irrlicht typed vector to a Bullet vector </summary>
			/// <param name="a_Vector">The vector as Irrlichts type </param>
			/// <returns> The vector as Bullets type </returns>
			static btVector3 toBulletVector(const irr::core::vector3df& a_Vector);

			/// <summary>
			/// Converts the given Bullet typed vector to an Irrlicht vector
			/// </summary>
			/// <param name="a_Vector">The vector as Bullets type</param>
			/// <returns> The vector as Irrlichts type </returns>
			static irr::core::vector3df toIrrlichtVector(const btVector3& a_Vector);
		private:			
			/// <summary> Executes the operations necessary before the actual simulation forward </summary>
			void prePhysicsUpdate() const;	

			/// <summary> 
			/// Synchronizes the transforms of the scenenodes with those of the RigidBodys in the 
			/// physics world 
			/// </summary>
			void synchronizeTransforms() const;
			
			/// <summary>
			/// Creates and places a RigidBody in the physics world
			/// </summary>
			/// <param name="a_Shape">The shape/collider to create the RigidBody for/attach it to</param>
			/// <param name="a_AttachedNode">The attached Irrlicht scenenode</param>
			/// <returns> The created RigidBody </returns>
			std::unique_ptr<RigidBody> createRigidBody(btCollisionShape* a_Shape,
				irr::scene::ISceneNode* a_AttachedNode) const;
		};
	}
}


