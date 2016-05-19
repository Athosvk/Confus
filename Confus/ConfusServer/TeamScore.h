#pragma once
#include <functional>

#include "../ConfusShared/TeamIdentifier.h"
#include "Networking/Connection.h"

namespace ConfusShared
{
	class Flag;
}

namespace ConfusServer
{
    /// <summary> Score class that keeps track the teams their score </summary>
    class TeamScore 
    {
    private:
        /// <summary> The max score a team gets to win a game.  </summary>
        /// <remarks> Is only set in the header file (TeamScore.h). </remarks>
        const int TeamMaxScore = 3;
        /// <summary> The Blue team's score. </summary>
        int m_BlueTeamScore = 0;
        /// <summary> The Red team's score. </summary> 
        int m_RedTeamScore = 0;
        /// <summary> The Connection class instance to send the score with. </summary>
        Networking::Connection* m_Connection;
        /// <summary> Reset game callback function. Will be called when the score hits the max score. Is used to reset the entire game. (Player, flag, maze and score) </summary>
        std::function<void()> m_ResetGameCallback;
    public:
		TeamScore(ConfusShared::Flag& a_BlueFlag, ConfusShared::Flag& a_RedFlag);
        /// <summary> TeamScore set connection. </summary>
        /// <param name="a_Connection"> The Connection class instance to send the score with. </param>
        void setConnection(Networking::Connection* a_Connection);
        /// <summary> Send the score to all the clients. </summary>
        void sendScoreToClients();
        /// <summary > Reset the score. </summary>
        void resetScore();
        /// <summary> Check the amount of points a team has. </summary>
        /// <returns> Returns the amount of points a team has. </returns>
        /// <param name="a_Team"> The team you want to know the score of. </param>
        int getPointsOfTeam(ConfusShared::ETeamIdentifier a_Team);
        /// <summary> Set the game reset callback </summary>
        void setResetCallback(const std::function<void()>& a_ResetGameCallback);
    private:
		/// <summary >Update the score of a team that scored. </summary>
		/// <param name="a_TeamScored"> The team that achieved a point. </param>
		void onTeamScored(ConfusShared::ETeamIdentifier a_TeamScored);
		
		/// <summary>Handles the event of the red team scoring, so that client side scoring can be adjusted</summary>
		void onRedTeamScored();
		
		/// <summary>Handles the event of the blue team scoring, so that client side scoring can be adjusted</summary>
		void onBlueTeamScored();

        /// <summary> TeamScore calls this to check if a team has score enough points to win. </summary>
        /// <returns> Returns if the team has enough points to win </returns>
        /// <param name="a_Team"> The team you want to know of if it has won. </param>
        bool teamHasWon(ConfusShared::ETeamIdentifier a_Team);
    };
}
