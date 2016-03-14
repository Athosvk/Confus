#pragma once
namespace irr 
{
    class IrrlichtDevice;
    namespace scene {
        class IAnimatedMeshSceneNode;
    }
}

namespace Confus 
{
	class Player;
}

namespace Confus 
{
// Team Identifier
/// <summary>
/// The Team's Identifier. A player has a team, flag has a team, ui has a team, etc.
/// </summary>
enum class TeamIdentifier 
{
	/// <summary>This object doesn't have a team identifier.</summary>
	NONE,
	/// <summary>This object has a red team identifier.</summary>
	TEAM_RED,
	/// <summary>This object has a green team identifier.</summary>
	TEAM_BLUE
};
enum class FlagEnum
{ 
    NONE,
    FLAG_BASE,
    FLAG_TAKEN,
    FLAG_DROPPED 
};

	//Flag Class, every flag should have this class, contains info about a flag
	/// Flag class with status and team id
	class Flag 
    {
    public:
        irr::scene::IAnimatedMeshSceneNode* m_FlagNode;
        irr::core::vector3df m_StartPosition;
        irr::core::vector3df m_StartRotation;
    private:
        FlagEnum m_FlagStatus = FlagEnum::FLAG_BASE;
        TeamIdentifier m_TeamIdentifier;
    public: 
        /// <summary>
        /// Flag class constructor
        /// </summary>
        /// <param name="a_SceneManager">The game's scene manager.</param>
        /// <param name="a_TeamIdentifier">The team's identifier the flag should have.</param>
        Flag(irr::IrrlichtDevice* a_Device, TeamIdentifier a_TeamIdentifier = TeamIdentifier::NONE);
        /// <summary>
        /// Flag class destructor
        /// </summary>
        ~Flag();
		/// Capture Flag
		void CaptureFlag(TeamIdentifier a_PlayerTeamID, Player* a_PlayerObject);
        void SetPosition(irr::core::vector3df a_Position = {0.f, 0.f, 0.f});
        void SetStartPosition(irr::core::vector3df a_Position);
        void SetStartRotation(irr::core::vector3df a_Position);
		void Drop(Player* a_PlayerObject);
		void Return();
		void Score(Player* a_PlayerObject);
        irr::video::SColor GetColor();

    private:
        void InitParticleSystem(irr::scene::ISceneManager* a_SceneManager);

	};
}
