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
enum class ETeamIdentifier 
{
	/// <summary>This object doesn't have a team identifier.</summary>
	NONE,
	/// <summary>This object has a red team identifier.</summary>
	TEAM_RED,
	/// <summary>This object has a green team identifier.</summary>
	TEAM_BLUE
};
enum class EFlagEnum
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
    private:
		irr::scene::IAnimatedMeshSceneNode* m_FlagNode;
		irr::core::vector3df m_StartPosition;
		irr::core::vector3df m_StartRotation;
        EFlagEnum m_FlagStatus = EFlagEnum::FLAG_BASE;
        ETeamIdentifier m_TeamIdentifier;
    public: 
        /// <summary>
        /// Flag class constructor
        /// </summary>
        /// <param name="a_SceneManager">The game's scene manager.</param>
        /// <param name="a_TeamIdentifier">The team's identifier the flag should have.</param>
        Flag(irr::IrrlichtDevice* a_Device, ETeamIdentifier a_TeamIdentifier = ETeamIdentifier::NONE);
        /// <summary>
        /// Flag class destructor
        /// </summary>
        ~Flag();
		/// Capture Flag
		void captureFlag(ETeamIdentifier a_PlayerTeamID, Player* a_PlayerObject);
        //void setPosition(irr::core::vector3df a_Position = {0.f, 0.f, 0.f});
        void setStartPosition(irr::core::vector3df a_Position);
        void setStartRotation(irr::core::vector3df a_Position);
		void drop(Player* a_PlayerObject);
		void returnToStartPos();
		void score(Player* a_PlayerObject);;

    private:
        void initParticleSystem(irr::scene::ISceneManager* a_SceneManager);
		void setColor(irr::video::IVideoDriver* a_VideoDriver);
		irr::video::SColor getColor();
	};
}
