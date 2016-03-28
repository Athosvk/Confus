#pragma once

namespace Confus 
{
class Player;
// Team Identifier
/// <summary>
/// The Team's Identifier. A player has a team, flag has a team, ui has a team, etc.
/// </summary>
enum class ETeamIdentifier 
{
	/// <summary>This object doesn't have a team identifier.</summary>
	None,
	/// <summary>This object has a red team identifier.</summary>
	TeamRed,
	/// <summary>This object has a blue team identifier.</summary>
	TeamBlue
};
enum class EFlagEnum
{ 
    None,
    FlagBase,
    FlagTaken,
    FlagDropped 
};

	//Flag Class, every flag should have this class, contains info about a flag
	/// Flag class with status and team id
	class Flag 
    {
    public:
        irr::scene::IAnimatedMeshSceneNode* FlagNode;
    private:		
		irr::core::vector3df* m_StartPosition;
		irr::core::vector3df* m_StartRotation;
		EFlagEnum* m_FlagStatus;
        ETeamIdentifier* m_TeamIdentifier;
    public: 
        /// <summary> Flag class constructor. </summary>
        /// <param name="a_SceneManager"> The game's scene manager. </param>
        /// <param name="a_TeamIdentifier"> The team's identifier the flag should have. </param>
        Flag(irr::IrrlichtDevice* a_Device, ETeamIdentifier a_TeamIdentifier);
        /// <summary> Flag class destructor </summary>
        ~Flag();
		/// Capture Flag
		void captureFlag(Player* a_PlayerObject);
        void setStartPosition(irr::core::vector3df a_Position);
        void setStartRotation(irr::core::vector3df a_Position);
		void drop(Player* a_PlayerObject);
		void returnToStartPosition();
		void score(Player* a_PlayerObject);
        void setCollisionTriangleSelector(irr::scene::ISceneManager* a_SceneManager, irr::scene::ITriangleSelector* a_TriangleSelector);

    private:
        void initParticleSystem(irr::scene::ISceneManager* a_SceneManager);
		void setColor(irr::video::IVideoDriver* a_VideoDriver);
		irr::video::SColor getColor();
	};
}
