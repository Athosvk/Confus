#pragma once
#include <Irrlicht/irrlicht.h>
#include <functional>

#include "Audio/PlayerAudioEmitter.h"

namespace Confus
{
	class Health
	{
	private:
		std::function<bool(irr::scene::ISceneNode* a_DamagedNode)> m_DeathCallback;
		irr::scene::ISceneNode* m_AttachedNode;
		/// <summary> The sound emitter that handles sounds for the player </summary>
		Audio::PlayerAudioEmitter* m_PlayerEmitter;
		int m_Health;
		int m_MaxHealth = 100;
	public:
		Health(Audio::PlayerAudioEmitter* a_AudioEmitter);
		void damage(int a_Damage);
		void heal(int a_Health);
		void setDeathCallback(const std::function<bool(irr::scene::ISceneNode* a_DamageNode)>& a_DeathCallback);
		int getHealth();
		
		/// <summary> Restores the health back to it's initial state </summary>
		void reset();
	};
}
