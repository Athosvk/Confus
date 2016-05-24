#pragma once
#include <Irrlicht/irrlicht.h>
#include <Bullet/btBulletDynamicsCommon.h>
#include <vector>
#include <memory>

#include "Collider.h"
#include "RigidBody.h"
#include "DebugDrawer.h"
#include "ECollisionFilter.h"
#include "CollisionRegistrar.h"

namespace ConfusShared
{
	namespace Physics
	{
		class BoxCollider;

		/// <summary> The world in which all, rigid body, physics simulations are performed by the physics engine </summary>
		class PhysicsWorld
		{
		private:
			/// <summary> A combination of a RigidBody and a collider </summary>
			struct ColliderPair
			{
			public:				
				/// <summary> 
				/// The (collider)shape of this collision structure,
				/// storing the instance of the bullet (shape) type so that we can edit its properties continously 
				/// </summary>
				std::unique_ptr<Collider> Shape;
				/// <summary> 
				/// The rigid body associated with the collider, allowing to modify properties such as the
				/// velocity
				/// </summary>
				std::unique_ptr<RigidBody> Body;

				/// <summary> Initializes a new instance of the <see cref="ColliderPair"/> struct </summary>
				/// <param name="a_Shape">The (collider)shape </param>
				/// <param name="a_RigidBody">The rigid body </param>
				ColliderPair(std::unique_ptr<Collider>&& a_Shape,
					std::unique_ptr<RigidBody>&& a_RigidBody);
			};

			/// <summary> The configuration settings for the physics world </summary>
			btDefaultCollisionConfiguration m_Configuration = btDefaultCollisionConfiguration();
			/// <summary> The collision dispatcher used by the physics engine </summary>
			btCollisionDispatcher m_Dispatcher = btCollisionDispatcher(&m_Configuration);
			/// <summary> The broadphase collision handler </summary>
			btDbvtBroadphase m_BroadPhaseInterface;
			/// <summary> The constraint solver for the physics world </summary>
			btSequentialImpulseConstraintSolver m_Solver;
			/// <summary> The internal, rigid body, simulation world of the physics engine </summary>
			btDiscreteDynamicsWorld m_World = btDiscreteDynamicsWorld(&m_Dispatcher, &m_BroadPhaseInterface,
				&m_Solver, &m_Configuration);
			/// <summary> The collider - rigid body pairs that are currently instantiated in the physics world </summary>
			std::vector<ColliderPair> m_Colliders;
			/// <summary> The debug drawer to draw debug information with </summary>
			DebugDrawer m_DebugDrawer;			
			/// <summary> The handles the callbacks of collisions </summary>
			CollisionRegistrar m_CollisionRegistrar;
		public:
			/// <summary>
			/// Initializes a new instance of the <see cref="PhysicsWorld" /> class
			/// </summary>
			/// <param name="a_Device">The currently active Irrlicht devcie</param>
			PhysicsWorld(irr::IrrlichtDevice* a_Device);

			/// <summary> Finalizes an instance of the <see cref="PhysicsWorld"/> class </summary>
			~PhysicsWorld();

			/// <summary>
			/// Creates and places a new box collider into the physics world for simulations
			/// </summary>
			/// <param name="a_Extents">The extents/dimensions of the collider to be created</param>
			/// <param name="a_AttachedNode">The node the collider is attached to</param>
			/// <param name="a_Type"> The type of the boxcollider, which group it belongs to</param>
			/// <param name="a_Mask"> The mask for ignoring collisions </param>
			/// <returns>
			/// A handle to the created box collider
			/// </returns>
			BoxCollider* createBoxCollider(irr::core::vector3df a_Extents, irr::scene::ISceneNode* a_AttachedNode,
				ECollisionFilter a_Group = ECollisionFilter::All, ECollisionFilter a_Mask = ECollisionFilter::None);

			/// <summary> Creates and places a new box collider from the scene node's bounding box into the physics world for simulations </summary>
			/// <param name="a_AttachedNode"> The node the collider is attached to </param>
			/// <returns> A handle to the created box collider </returns>
			BoxCollider* createBoxCollider(irr::scene::ISceneNode* a_AttachedNode, ECollisionFilter a_Group = ECollisionFilter::Other,
				ECollisionFilter a_Mask = ECollisionFilter::All);

			/// <summary> Advanced the physics simulation based on the entered delta time value </summary>
			/// <param name="a_DeltaTime"> 
			/// The time with which to advance the physics simulation usually equal to the interval between two 
			/// fixed/physics updates
			/// </param>
			void stepSimulation(float a_DeltaTime);

			/// <summary> Draws the debug information of the world </summary>
			void drawDebugInformation();

			/// <summary> Converts the given Irrlicht typed vector to a Bullet vector </summary>
			/// <param name="a_Vector">The vector as Irrlichts type </param>
			/// <returns> The vector as Bullets type </returns>
			static btVector3 toBulletVector(const irr::core::vector3df& a_Vector);

			/// <summary>
			/// Converts the given Bullet typed vector to an Irrlicht vector
			/// </summary>
			/// <param name="a_Vector"> The vector as Bullets type </param>
			/// <returns> The vector as Irrlichts type </returns>
			static irr::core::vector3df toIrrlichtVector(const btVector3& a_Vector);
		private:			
			/// <summary> Executes the operations necessary before the actual simulation step occurs </summary>
			void prePhysicsUpdate() const;	

			/// <summary> Executes the operations necessary after the simulation step has occurred </summary>
			void postPhysicsUpdate() const;
			
			/// <summary>
			/// Creates and places a RigidBody in the physics world
			/// </summary>
			/// <param name="a_Shape">The shape/collider to create the RigidBody for/attach it to</param>
			/// <param name="a_AttachedNode">The attached Irrlicht scenenode</param>
			/// <returns> The created RigidBody </returns>
			/// <remarks> The created Rigid Body has a mass of one and is dynamic by default </remarks>
			std::unique_ptr<btRigidBody> createRigidBody(btCollisionShape* a_Shape,
				irr::scene::ISceneNode* a_AttachedNode, ECollisionFilter a_Group, ECollisionFilter a_Mask);
		};
	}
}


