#pragma once
#include "../EnumUtility.h"

namespace ConfusShared
{
	namespace Physics
	{		
		/// <summary>
		/// Represents one or a combination of collision filters that can be used to exclude collisions between certain groups
		/// of objects, both for performance and excluded collisions
		/// </summary>
		/// <remarks> 
		/// The collision filter needs to be set for both objects/object groups in order for a collision to be registered. For example,
		/// the player needs to have the MoveableWall filter set as well as the MoveableWall having to have the Player filter set
		/// </remarks>
		enum class ECollisionFilter : short
		{			
			/// <summary> Means this object either belongs to no collision filter group or has no interaction at all </summary>
			None = 0,			
			/// <summary> Means this object is either a Moveable Wall or collides with Moveable Walls </summary>
			MoveableWall = 1,			
			/// <summary> Means this object is either a Player or collides with Players </summary>
			Player = 2,			
			/// <summary> Means this is either level static geometry or this object collides with level static geometry </summary>
			LevelStatic = 4,			
			/// <summary> Means this is either some kind of interactable or this object collides with interactables </summary>
			Interactable = 8,			
			/// <summary> Means this object is either of unspecified type or this object collides with objects of unspecified types </summary>
			Other = 16,			
			/// <summary> Means this object is either of all types or collides with all groups of objects </summary>
			/// <remarks> SHould only be used as the filter, not the group this object belongs to </remarks>
			All = None | MoveableWall | Player | LevelStatic | Interactable | Other
		};
		EnumFlagOperators(ECollisionFilter);
	}
}
