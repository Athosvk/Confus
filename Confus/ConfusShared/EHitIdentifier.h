#pragma once

namespace ConfusShared
{
	/// <summary> The Team's Identifier. There are two teams, blue and red. A player has a team, flag has a team, UI has a team, etc. </summary>
	enum class EHitIdentifier
	{		
		/// <summary>Represents a light attack</summary>
		Light = 0,		
		/// <summary>Represents a heavy attack</summary>
		Heavy = 1 << 0,		
		/// <summary>Represents a light backstab attack, for when a player has been attacked from the rear</summary>
		LightBackstab = 1 << 1,		
		/// <summary>Represents a heavy backstab attack, for when a palyer has been attacked from the rear </summary>
		HeavyBackstab = 1 << 2
	};
}

using ConfusShared::EHitIdentifier;