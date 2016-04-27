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
		
		class CollisionRegistrar
		{
		private:
			struct CollisionInfo
			{
				BoxCollider* ColliderA;
				BoxCollider* ColliderB;

				CollisionInfo(BoxCollider* a_ColliderA, BoxCollider* a_ColliderB);

				bool operator==(const CollisionInfo& a_Other);
			};

			std::vector<CollisionInfo> m_PreviousCollisions;
			std::vector<CollisionInfo> m_Collisions;
			std::unordered_map<BoxCollider*, std::function<void(BoxCollider* a_Other)>> m_TriggerEnterCallbacks;
			btCollisionWorld* m_CollisionWorld;

		public:
			CollisionRegistrar(btCollisionWorld* a_World);

			void setCallback(BoxCollider* a_Collider, std::function<void(BoxCollider* a_Other)>& 
				a_Callback);
			void setCallback(BoxCollider* a_Collider, std::function<void(BoxCollider* a_Other)>&& 
				a_Callback);
			void onPostPhysicsUpdate();
		private:
			void handleCollision(BoxCollider* a_Collider, BoxCollider* a_Other);
		};
	}
}
