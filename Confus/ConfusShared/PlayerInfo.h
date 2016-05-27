#pragma pack(push, 1)
#include <Irrlicht/vector3d.h>

namespace ConfusShared
{
#pragma pack(push, 1)
	/// <summary> Represents the state of the keys pressed by the player </summary>
	struct  PlayerInfo
	{
		long long playerID = 0;
		irr::core::vector3df position = irr::core::vector3df(0.f);
		irr::core::vector3df rotation = irr::core::vector3df(0.f);
	};
#pragma pack(pop)
}
