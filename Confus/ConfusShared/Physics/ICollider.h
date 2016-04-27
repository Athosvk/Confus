#pragma once
namespace Confus
{
	namespace Physics
	{
		class RigidBody;

		/// <summary> A generic interface that all colliders in the physics world need to implement </summary>
		//To do: Change name to Collider after refactor
		class ICollider
		{
		private:
			RigidBody* m_RigidBody;
		protected:
			ICollider(RigidBody* a_RigidBody);
		public:
			virtual ~ICollider() = default;

			RigidBody* getRigidBody();
		};
	}
}