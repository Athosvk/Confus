#pragma once

namespace ConfusShared
{
	/// <summary> The Team's Identifier. There are two teams, blue and red. A player has a team, flag has a team, UI has a team, etc. </summary>
	enum class EHitIdentifier
	{
		Light = 0,
		Heavy = 1 << 0,
		LightBackstab = 1 << 1,
		HeavyBackstab = 1 << 2

	};
}


using ConfusShared::EHitIdentifier;