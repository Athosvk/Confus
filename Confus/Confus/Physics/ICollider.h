#pragma once

class btCollisionShape;

namespace Confus
{
	namespace Physics
	{
		class ICollider
		{
		private:
			btCollisionShape* m_ColisionShape;


		protected:
			ICollider();
		};
	}
}