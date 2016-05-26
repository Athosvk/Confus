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
		/// <summary>Sets the team score for this client instance</summary>
		/// <param name="a_TeamIdentifier">The team to set the score for</param>
		/// <param name="a_Score">The new score</param>
		void setTeamScore(ConfusShared::ETeamIdentifier a_TeamIdentifier, int a_Score);

		/// <summary>Gets the team score of the specified team</summary>
		/// <param name="a_TeamIdentifier">The team to get the score of</param>
		/// <returns>The score of that team</returns>
		int getTeamScore(ConfusShared::ETeamIdentifier a_TeamIdentifier) const;
	};
}
