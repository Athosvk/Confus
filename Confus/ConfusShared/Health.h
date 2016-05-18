#pragma once
#include <Irrlicht/irrlicht.h>
#include <functional>

#include "../ConfusShared/EHitIdentifier.h"
#include "Delegate.h"

namespace ConfusShared
{
	class Health
	{
	public:
		Delegate<void(EHitIdentifier a_HitIdentifier)> DamageEvent;
	private:
		std::function<bool(irr::scene::ISceneNode* a_DamagedNode)> m_DeathCallback;
		irr::scene::ISceneNode* m_AttachedNode;
		int m_Health;
		int m_MaxHealth = 100;
	public:
		Health();
		void damage(int a_Damage, EHitIdentifier a_HitIdentifier);
		void heal(int a_Health);
		void setDeathCallback(const std::function<bool(irr::scene::ISceneNode* a_DamageNode)>& a_DeathCallback);
		int getHealth();
		
		/// <summary> Restores the health back to it's initial state </summary>
		void reset();
	};
}
