#include <iostream>
#include <memory>
#include <RakNet/BitStream.h>
#include <RakNet\MessageIdentifiers.h>
#include <string>

#include "TeamScore.h"
#define DEBUG_CONSOLE
#include "../ConfusShared/Debug.h"

namespace ConfusServer 
{
    void TeamScore::setConnection(Networking::Connection* a_Connection)
    {
        m_Connection = a_Connection;
    }

    void TeamScore::sendScoreToClients()
    {
        RakNet::BitStream bitStream;
        bitStream.Write(static_cast<RakNet::MessageID>(Networking::EPacketType::ScoreUpdate));

        bitStream.Write(m_RedTeamScore);
		bitStream.Write(m_BlueTeamScore);

        m_Connection->broadcastBitstream(bitStream);
    }

    void TeamScore::teamScoredPoint(ConfusShared::ETeamIdentifier a_TeamScored, int a_IncreasementValue)
    {
        if(a_TeamScored == ConfusShared::ETeamIdentifier::TeamBlue)
        {
            m_BlueTeamScore += a_IncreasementValue;
        } 
        else if(a_TeamScored == ConfusShared::ETeamIdentifier::TeamRed)
        {
            m_RedTeamScore += a_IncreasementValue;
        }
        if(teamHasWon(a_TeamScored))
        {
            RakNet::BitStream bitStream;
            bitStream.Write(static_cast<RakNet::MessageID>(Networking::EPacketType::EndOfGame));
            bitStream.Write(a_TeamScored);
            m_Connection->broadcastBitstream(bitStream);
            std::cout << "Team has won: " << static_cast<int>(a_TeamScored) << std::endl;
            m_ResetGameCallback();
        }
        std::cout << "Score updated\tRed score: " << m_RedTeamScore << "\t Blue score: " << m_BlueTeamScore << std::endl;
        sendScoreToClients();
    }

    void TeamScore::resetScore()
    {
        m_RedTeamScore = 0;
        m_BlueTeamScore = 0;
    }

    int TeamScore::getPointsOfTeam(ConfusShared::ETeamIdentifier a_Team)
    {
        int scoreAmount = 0;
        switch(a_Team)
        {
		case ConfusShared::ETeamIdentifier::TeamBlue:
            scoreAmount = m_BlueTeamScore;
            break;
		case ConfusShared::ETeamIdentifier::TeamRed:
            scoreAmount = m_RedTeamScore;
            break;
        default:
            throw new std::logic_error("Team does not exist");
            break;
        }
        return scoreAmount;
    }

    void TeamScore::setResetCallback(const std::function<void()>& a_ResetGameCallback)
    {
        m_ResetGameCallback = a_ResetGameCallback;
    }

    bool TeamScore::teamHasWon(ConfusShared::ETeamIdentifier a_Team)
    {
        return getPointsOfTeam(a_Team) == TeamMaxScore;
    }
}
