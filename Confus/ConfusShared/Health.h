#pragma once
#include <Irrlicht/irrlicht.h>
#include <functional>

#include "../ConfusShared/EHitIdentifier.h"
#include "Delegate.h"

namespace ConfusShared
{	
	/// <summary>
	/// Represents a health instance, mostly used for the player, so that such instances can be killed 
	/// gradually
	/// </summary>
	class Health
	{
	public:		
		/// <summary>
		/// The event called once the instance associated with this health object gets damaged
		/// so that sounds can be played and variables can be updated
		/// </summary>
		Delegate<void(EHitIdentifier a_HitIdentifier)> DamageEvent;
	private:		
		/// <summary>
		/// The callback called when the instance associated with this health object is killed,
		/// so that variables can be reset and the instance can potentially be respawned
		/// </summary>
		std::function<bool(irr::scene::ISceneNode* a_DamagedNode)> m_DeathCallback;
		/// <summary>The node associated with the health object</summary>
		irr::scene::ISceneNode* m_AttachedNode;
		/// <summary>The current health of this instance</summary>
		int m_Health;		
		/// <summary>The maximum and starting health of this instance</summary>
		int m_MaxHealth = 100;
	public:		
		/// <summary>Initializes a new instance of the <see cref="Health"/> class.</summary>
		Health();
		
		/// <summary>Deals damage to this health insance</summary>
		/// <param name="a_Damage">The damage to deal</param>
		/// <param name="a_HitIdentifier">The type of damage that was done</param>
		void damage(int a_Damage, EHitIdentifier a_HitIdentifier);
				
		/// <summary>Heals this instance by the specified amount</summary>
		/// <param name="a_Health">The health to heal by</param>
		void heal(int a_Health);
		
		/// <summary>Sets the death callback.</summary>
		/// <param name="a_DeathCallback">The new callback</param>
		void setDeathCallback(const std::function<bool(irr::scene::ISceneNode* a_DamageNode)>& a_DeathCallback);
				
		/// <summary>Gets the current health value</summary>
		/// <returns>The current health value</returns>
		int getHealth();
		
		/// <summary> Restores the health back to it's initial state </summary>
		void reset();
	};
}
