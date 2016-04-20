#include "Health.h"

namespace Confus
{
	Health::Health()
	{
		m_Health = m_MaxHealth;
	}

	void Health::damage(int a_Damage)
	{
		if (a_Damage > 0)
		{
			m_Health -= a_Damage;
			if (m_Health <= 0)
			{
				m_Health = 0;
				m_DeathCallback(m_AttachedNode);
			}
		}
	}

	void Health::heal(int a_Health)
	{
		if (a_Health > 0)
		{
			m_Health += a_Health;
			if (m_Health > m_MaxHealth)
			{
				m_Health = m_MaxHealth;
			}
		}
	}

	void Health::setDeathCallback(const std::function<bool(irr::scene::ISceneNode* a_DamageNode)>& a_DeathCallback)
	{
		m_DeathCallback = a_DeathCallback;
	}

	int Health::getHealth()
	{
		return m_Health;
	}
}
