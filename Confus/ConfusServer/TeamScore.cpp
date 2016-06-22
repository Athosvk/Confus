#include <iostream>
#include <RakNet/BitStream.h>
#include <string>

#include "TeamScore.h"
#define DEBUG_CONSOLE
#include "../ConfusShared/Debug.h"
#include "../ConfusShared/Flag.h"
#include "../ConfusShared/PacketType.h"

namespace ConfusServer 
{
	TeamScore::TeamScore(ConfusShared::Flag& a_BlueFlag, ConfusShared::Flag& a_RedFlag)
	{
		a_BlueFlag.addScoreCallback([this]()
		{
			onBlueTeamScored();
		});
		a_RedFlag.addScoreCallback([this]()
		{
			onRedTeamScored();
		});
	}

	void TeamScore::setConnection(Networking::Connection* a_Connection)
    {
        m_Connection = a_Connection;
		sendScoreToClients();
    }

    void TeamScore::sendScoreToClients()
    {
        RakNet::BitStream bitStream;
        bitStream.Write(static_cast<RakNet::MessageID>(ConfusShared::Networking::EPacketType::ScoreUpdate));

        bitStream.Write(m_RedTeamScore);
		bitStream.Write(m_BlueTeamScore);

        m_Connection->broadcastPacket(&bitStream);
    }

    void TeamScore::onTeamScored(ConfusShared::ETeamIdentifier a_TeamScored)
    {
        if(teamHasWon(a_TeamScored))
        {
            RakNet::BitStream bitStream;
            bitStream.Write(static_cast<RakNet::MessageID>(ConfusShared::Networking::EPacketType::EndOfGame));
            bitStream.Write(a_TeamScored);
            m_Connection->broadcastPacket(&bitStream);
            std::cout << "Team has won: " << static_cast<int>(a_TeamScored) << std::endl;
            m_ResetGameCallback();
        }
        std::cout << "Score updated\tRed score: " << m_RedTeamScore << "\t Blue score: " << m_BlueTeamScore << std::endl;
        sendScoreToClients();
    }

	void TeamScore::onRedTeamScored()
	{
		m_RedTeamScore += 1;
		onTeamScored(ConfusShared::ETeamIdentifier::TeamRed);
	}

	void TeamScore::onBlueTeamScored()
	{
		m_BlueTeamScore += 1;
		onTeamScored(ConfusShared::ETeamIdentifier::TeamBlue);
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
            throw std::logic_error("Team does not exist");
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
