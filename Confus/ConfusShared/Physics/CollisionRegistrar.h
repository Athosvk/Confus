#pragma once
#include <Bullet/btBulletCollisionCommon.h>
#include <unordered_map>
#include <vector>
#include <functional>

namespace Confus
{
	namespace Physics
	{
		class BoxCollider;

		/// <summary>
		/// Keeps track of all the collisions that have occured during the physics simulation, in order to call the correct callbacks
		/// </summary>
		class CollisionRegistrar
		{
		private:			
			/// <summary> Keeps track of the information about an occured collision </summary>
			struct CollisionInfo
			{				
				/// <summary> The first collider involved in the collision </summary>
				BoxCollider* ColliderA;				
				/// <summary> The other collider involved in the collision </summary>
				BoxCollider* ColliderB;
				
				/// <summary> Initializes a new instance of the <see cref="CollisionInfo"/> struct  </summary>
				/// <param name="a_ColliderA">The first collider involved in the collision </param>
				/// <param name="a_ColliderB">The other collider involved in the collision </param>
				CollisionInfo(BoxCollider* a_ColliderA, BoxCollider* a_ColliderB);
				
				/// <summary> Compares whether two instances of CollisionInfo are equal </summary>
				/// <param name="a_Other">The CollisionInfo instance to check against </param>
				/// <returns> Whether this instance is eqaul to the given CollisionInfo instance </returns>
				bool operator==(const CollisionInfo& a_Other);
			};
			
			/// <summary> The collisions that occurred during the previous phsyics simulation step </summary>
			std::vector<CollisionInfo> m_PreviousCollisions;			
			/// <summary> The collisions that occurred during the current physics simulation step </summary>
			std::vector<CollisionInfo> m_Collisions;			
			/// <summary> The registered callbacks for the TriggerEnter event </summary>
			std::unordered_map<BoxCollider*, std::function<void(BoxCollider* a_Other)>> m_TriggerEnterCallbacks;			
			/// <summary> The collision world to keep track of collisions </summary>
			btCollisionWorld* m_CollisionWorld;

		public:			
			/// <summary> Initializes a new instance of the <see cref="CollisionRegistrar"/> class. </summary>
			/// <param name="a_World">The collision world to keep track of collisions of </param>
			CollisionRegistrar(btCollisionWorld* a_World);
			
			/// <summary> Sets the trigger enter callback for the given collider </summary>
			/// <param name="a_Collider">The collider to set the callback of </param>
			/// <param name="a_Callback">The callback to use once a trigger enter event has occurred </param>
			void setTriggerEnterCallback(BoxCollider* a_Collider, std::function<void(BoxCollider* a_Other)>&
				a_Callback);

			/// <summary> Sets the trigger enter callback for the given collider </summary>
			/// <param name="a_Collider">The collider to set the callback of </param>
			/// <param name="a_Callback">The callback to use once a trigger enter event has occurred </param>
			void setTriggerEnterCallback(BoxCollider* a_Collider, std::function<void(BoxCollider* a_Other)>&& 
				a_Callback);

			/// <summary> Executes the actiosn that need to occur after the physics update </summary>
			void onPostPhysicsUpdate();
		private:			
			/// <summary> Handles the collision between given colliders </summary>
			/// <param name="a_Collider">The first collider involved in the collision </param>
			/// <param name="a_Other">The other collider involved in the collision </param>
			void handleCollision(BoxCollider* a_Collider, BoxCollider* a_Other);
		};
	}
}
