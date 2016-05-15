#pragma once

namespace ConfusShared 
{
	/// <summary>
	/// The state the player is in. Alive, Light attack, Heavy attack, dead, etc.
	/// </summary>
    enum class EPlayerState : unsigned char
    {
        Idle = 0,
		HeavyAttacking = 1u, 
		LightAttacking = 2u
    };
}
using ConfusShared::EPlayerState;
