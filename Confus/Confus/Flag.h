#pragma once
namespace irr {
	namespace scene {
		class ISceneManager;
	}
}

namespace Confus {
	class Player;
}

namespace Confus {
// Team Identifier
/// <summary>
/// The Team's Identifier. A player has a team, flag has a team, ui has a team, etc.
/// </summary>
enum TeamIdentifier {
	/// <summary>This object doesn't have a team identifier.</summary>
	NONE,
	/// <summary>This object has a red team identifier.</summary>
	TEAM_RED,
	/// <summary>This object has a green team identifier.</summary>
	TEAM_GREEN
};

	//Flag Class, every flag should have this class, contains info about a flag
	/// Flag class with status and team id
	class Flag {
		enum FlagEnum { NONE, FLAG_BASE, FLAG_TAKEN, FLAG_DROPPED };
	public: 
		/// Capture Flag
		void CaptureFlag(TeamIdentifier a_PlayerTeamID, Player* a_PlayerObject);
		void Drop(Player* a_PlayerObject);
		void Return();
		void Score(Player* a_PlayerObject);
	private:
		FlagEnum m_FlagStatus = FLAG_BASE;
		TeamIdentifier m_TeamIdentifier;
	public:
		/// <summary>
		/// Flag class constructor
		/// </summary>
		/// <param name="a_SceneManager">The game's scene manager.</param>
		/// <param name="a_TeamIdentifier">The team's identifier the flag should have.</param>
		Flag(irr::scene::ISceneManager* a_SceneManager, TeamIdentifier a_TeamIdentifier = TeamIdentifier::NONE);
		/// <summary>
		/// Flag class destructor
		/// </summary>
		~Flag();
	};
}
