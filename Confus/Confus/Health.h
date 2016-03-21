#pragma once
#include <Irrlicht/irrlicht.h>
#include <functional>

namespace Confus
{
	class Health
	{
	private:
		std::function<bool(irr::scene::ISceneNode* a_DamagedNode)> m_DeathCallback;
		irr::scene::ISceneNode* m_AttachedNode;

	public:
		int m_Health = 100;
		void damage(int a_Damage);
		void heal(int a_Heal);
		void setDeathCallback(const std::function<bool(irr::scene::ISceneNode* a_DamageNode)>& a_DeathCallback);
		void setHealth(int a_Health);
		int getHealth();
	};
}