#include "Health.h"

namespace Confus
{
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
			else if (m_Health <= 50)
			{

			}
		}
	}

	void Health::heal(int a_Heal)
	{
		if (a_Heal > 0)
		{
			m_Health += a_Heal;
			if (m_Health > 100)
			{
				m_Health = 100;
			}
		}
	}

	void Health::setDeathCallback(const std::function<bool(irr::scene::ISceneNode* a_DamageNode)>& a_DeathCallback)
	{
		m_DeathCallback = a_DeathCallback;
	}

	void Health::setHealth(int a_Health)
	{
		m_Health = a_Health;
	}

	int Health::getHealth()
	{
		return m_Health;
	}
}