#include <iostream>
#include <memory>
#include <RakNet/BitStream.h>
#include <RakNet\MessageIdentifiers.h>
#include <string>

#include "TeamScore.h"
#include "Debug.h"
#define DEBUG_CONSOLE

namespace ConfusServer 
{
    void TeamScore::setConnection(Networking::Connection* a_Connection)
    {
        m_Connection = a_Connection;
    }

    void TeamScore::sendScore()
    {
        RakNet::BitStream bitStream;
        bitStream.Write(static_cast<RakNet::MessageID>(ID_SCORE_UPDATE));

        std::string test = std::to_string(static_cast<int>(ETeamIdentifier::TeamRed)) + std::to_string(m_RedTeamScore) + std::to_string(static_cast<int>(ETeamIdentifier::TeamBlue)) + std::to_string(m_BlueTeamScore);
        bitStream.Write(test.c_str());

        m_Connection->sendPacketToAllClients(bitStream);
    }

    void TeamScore::sendWin()
    {
        //TODO: Send win to clients
    }

    void TeamScore::teamScoredPoint(ETeamIdentifier a_TeamScored, int a_IncreasementValue)
    {
        if(a_TeamScored == ETeamIdentifier::TeamBlue)
        {
            m_BlueTeamScore += a_IncreasementValue;
        } 
        else if(a_TeamScored == ETeamIdentifier::TeamRed)
        {
            m_RedTeamScore += a_IncreasementValue;
        }
        if(teamHasWon(a_TeamScored))
        {
            std::cout << "Team " << static_cast<int>(a_TeamScored) << " was the winner!" << std::endl;
            sendWin();
        }
        sendScore();
    }

    int TeamScore::getPointsOfTeam(ETeamIdentifier a_Team) 
    {
        int scoreAmount = 0;
        switch(a_Team)
        {
        case ETeamIdentifier::TeamBlue:
            scoreAmount = m_BlueTeamScore;
            break;
        case ETeamIdentifier::TeamRed:
            scoreAmount = m_RedTeamScore;
            break;
        default:
            std::cout << "Error: Team does not exist.";
            break;
        }
        return scoreAmount;
    }

    bool TeamScore::teamHasWon(ETeamIdentifier a_Team)
    {
        return getPointsOfTeam(a_Team) == TeamMaxScore;
    }
}
