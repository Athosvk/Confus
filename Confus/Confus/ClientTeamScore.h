#pragma once

#include "../Common/TeamIdentifier.h"

namespace Confus
{
	/// <summary> Class to set the team score with </summary>
	class ClientTeamScore 
    {
	private:
		/// <summary> The red team score, static so it can be accessed from anywhere. </summary>
		static int RedTeamScore;
		/// <summary> The blue team score, static so it can accessed from anywhere. </summary>
		static int BlueTeamScore;
    public:
        static void setTeamScore(ETeamIdentifier a_TeamIdentifier, int a_Score);
        static int getTeamScore(ETeamIdentifier a_TeamIdentifier);
	};
}
