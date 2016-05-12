#include <stdexcept>

#include "ClientTeamScore.h"


namespace Confus
{
    int ClientTeamScore::RedTeamScore = 0;
    int ClientTeamScore::BlueTeamScore = 0;

    void ClientTeamScore::setTeamScore(ConfusShared::ETeamIdentifier a_TeamIdentifier, int a_Score)
    {
         switch(a_TeamIdentifier)
        {
        case ConfusShared::ETeamIdentifier::TeamRed:
            RedTeamScore = a_Score;
            break;
        case ConfusShared::ETeamIdentifier::TeamBlue:
            BlueTeamScore = a_Score;
            break;
        default:
            throw std::logic_error("Team does not exist");
            break;
        }
    }

    int ClientTeamScore::getTeamScore(ConfusShared::ETeamIdentifier a_TeamIdentifier)
    {
        int teamScoreValue = 0;
        switch(a_TeamIdentifier)
        {
        case ConfusShared::ETeamIdentifier::TeamRed:
            teamScoreValue = RedTeamScore;
            break;
        case ConfusShared::ETeamIdentifier::TeamBlue:
            teamScoreValue = BlueTeamScore;
            break;
        default:
            throw std::logic_error("Team does not exist");
            break;
        }
        return teamScoreValue;
    }
}
