#pragma once

namespace ConfusShared
{
    /// <summary> The Team's Identifier. There are two teams, blue and red. A player has a team, flag has a team, UI has a team, etc. </summary>
    enum class ETeamIdentifier : unsigned char
    {
        /// <summary> This object doesn't have a team identifier. </summary>
        None = 0u,
        /// <summary> This object has a red team identifier. </summary>
        TeamRed = 1u,
        /// <summary> This object has a blue team identifier. </summary>
        TeamBlue = 2
    };
}


using ConfusShared::ETeamIdentifier;