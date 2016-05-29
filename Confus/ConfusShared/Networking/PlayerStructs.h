#include <Irrlicht/irrlicht.h>

#include "../PlayerInputState.h"
#include "../Player.h"

namespace ConfusShared
{
    namespace Networking
    {
        // Origin of these packets is from the server
        namespace Server
        {
#pragma pack(push, 1)
            /// <summary> Represents the information of the server from the client that is networked </summary>
            struct PlayerUpdate
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

            /// <summary> Represents the all the information of a player, that is needed to add him to the world </summary>
            struct NewPlayer
            {
                /// <summary> The guid of a player</summary>
                long long ID = 0;
                /// <summary> The team of a player</summary>
                ETeamIdentifier Team;
                /// <summary> The position of a player</summary>
                irr::core::vector3df Position = irr::core::vector3df(0.f);
                /// <summary> The rotation of a player</summary>
                irr::core::vector3df Rotation = irr::core::vector3df(0.f);
				
				/// <summary>Initializes a new instance of the <see cref="NewPlayer"/> struct.</summary>
				/// <param name="a_Player">The player to construct the info from</param>
				NewPlayer(ConfusShared::Player* a_Player);

				/// <summary>Initializes a new instance of the <see cref="NewPlayer"/> struct.</summary>
				NewPlayer() = default;
            };
        }

        // Origin of this packet is from the client
        namespace Client
        {
            /// <summary> Represents the information of a client that is networked </summary>
            struct PlayerUpdate
            {
                /// <summary> The guid of a player</summary>
                long long ID = 0;
                /// <summary> The rotation of a player</summary>
                irr::core::vector3df Rotation = irr::core::vector3df(0.f);
                /// <summary> The input state of a player</summary>
                PlayerInputState InputState;
            };
#pragma pack(pop)
        }
    }   
}

