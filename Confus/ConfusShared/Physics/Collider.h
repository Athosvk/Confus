#pragma once
namespace Confus
{
	namespace Physics
	{
		class RigidBody;

		/// <summary> A generic interface that all colliders in the physics world need to implement </summary>
		class Collider
		{
		private:
			RigidBody* m_RigidBody;
		protected:
			Collider(RigidBody* a_RigidBody);
		public:
			virtual ~Collider() = default;

			RigidBody* getRigidBody();
		};
	}
}