#pragma once

namespace ConfusShared
{
    /// <summary> The Team's Identifier. There are two teams, blue and red. A player has a team, flag has a team, UI has a team, etc. </summary>
    enum class ETeamIdentifier
    {
        /// <summary> This object doesn't have a team identifier. </summary>
        None = 0,
        /// <summary> This object has a red team identifier. </summary>
        TeamRed = 1 << 0,
        /// <summary> This object has a blue team identifier. </summary>
        TeamBlue = 1 << 1
    };
}


using ConfusShared::ETeamIdentifier;