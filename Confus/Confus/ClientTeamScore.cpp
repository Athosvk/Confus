#include <stdexcept>

#include "ClientTeamScore.h"

namespace Confus
{
    void ClientTeamScore::setTeamScore(ConfusShared::ETeamIdentifier a_TeamIdentifier, int a_Score)
    {
        switch(a_TeamIdentifier)
        {
        case ConfusShared::ETeamIdentifier::TeamRed:
			m_RedTeamScore = a_Score;
            break;
        case ConfusShared::ETeamIdentifier::TeamBlue:
			m_BlueTeamScore = a_Score;
            break;
        default:
            throw std::logic_error("Team does not exist");
            break;
        }
    }

    int ClientTeamScore::getTeamScore(ConfusShared::ETeamIdentifier a_TeamIdentifier) const
    {
        int teamScoreValue = 0;
        switch(a_TeamIdentifier)
        {
        case ConfusShared::ETeamIdentifier::TeamRed:
            teamScoreValue = m_RedTeamScore;
            break;
        case ConfusShared::ETeamIdentifier::TeamBlue:
            teamScoreValue = m_BlueTeamScore;
            break;
        default:
            throw std::logic_error("Team does not exist");
            break;
        }
        return teamScoreValue;
    }
}
