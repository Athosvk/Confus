#pragma once

namespace ConfusShared 
{
	/// <summary>
	/// The state the player is in. Alive, Light attack, Heavy attack, dead, etc.
	/// </summary>
    enum class EPlayerState : unsigned char
    {
        Idle = 0,
		Alive = 1u,
		CarryingFlag = 2u,
		HeavyAttacking = 3u, 
		LightAttacking = 4u,
		Dead = 5u
    };
}
using ConfusShared::EPlayerState;
