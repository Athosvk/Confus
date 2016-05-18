#pragma once

#include "../ConfusShared/TeamIdentifier.h"

namespace Confus
{
	/// <summary> Class to set the team score with </summary>
	class ClientTeamScore 
    {
	private:
		/// <summary> The red team score, static so it can be accessed from anywhere. </summary>
		int m_RedTeamScore;
		/// <summary> The blue team score, static so it can accessed from anywhere. </summary>
		int m_BlueTeamScore;
    public:
        void setTeamScore(ConfusShared::ETeamIdentifier a_TeamIdentifier, int a_Score);
        int getTeamScore(ConfusShared::ETeamIdentifier a_TeamIdentifier) const;
	};
}
