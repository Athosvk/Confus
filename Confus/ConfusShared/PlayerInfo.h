#pragma pack(push, 1)
#include "Player.h"
#include "PlayerInputState.h"

namespace ConfusShared
{
#pragma pack(push, 1)
	/// <summary> Represents the information of the server from the client that is networked </summary>
	struct  PlayerUpdateFromServer
	{
		/// <summary> The guid of a player</summary>
		long long ID = 0;
		/// <summary> The position of a player</summary>
		irr::core::vector3df Position = irr::core::vector3df(0.f);
		/// <summary> The rotation of a player</summary>
		irr::core::vector3df Rotation = irr::core::vector3df(0.f);
		/// <summary> The attacking state of a player</summary>
		EPlayerState State = EPlayerState::Alive;
	};
#pragma pack(pop)

#pragma pack(push, 1)
	/// <summary> Represents the information of a client that is networked </summary>
	struct  PlayerUpdateFromClient
	{
		/// <summary> The guid of a player</summary>
		long long ID = 0;
		/// <summary> The rotation of a player</summary>
		irr::core::vector3df Rotation = irr::core::vector3df(0.f);
		/// <summary> The input state of a player</summary>
		PlayerInputState InputState;
	};
#pragma pack(pop)

#pragma pack(push, 1)
	/// <summary> Represents the all the information of a player, that is needed to add him to the world </summary>
	struct  NewPlayerFromServer
	{
		/// <summary> The guid of a player</summary>
		long long ID = 0;
		/// <summary> The team of a player</summary>
		ETeamIdentifier Team;
		/// <summary> The position of a player</summary>
		irr::core::vector3df Position = irr::core::vector3df(0.f);
		/// <summary> The rotation of a player</summary>
		irr::core::vector3df Rotation = irr::core::vector3df(0.f);
	};
#pragma pack(pop)
}
