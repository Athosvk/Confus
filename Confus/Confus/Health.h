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
		int m_Health;
		int m_MaxHealth = 100;
	public:
		Health();
		void damage(int a_Damage);
		void heal(int a_Health);
		void setDeathCallback(const std::function<bool(irr::scene::ISceneNode* a_DamageNode)>& a_DeathCallback);
		int getHealth();
	};
}
