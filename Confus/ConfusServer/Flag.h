#pragma once
#include "../Common/TeamIdentifier.h"
#include "TeamScore.h"

namespace ConfusServer 
{
    class Player;
    class Collider;

    /// <summary> The Flag's status. A player can have flagtaken or no flag. A flag can be taken, dropped or at base. </summary>
    enum class EFlagEnum
    { 
        None, ///< Player is carrying nothing		  
	    FlagBase, ///< Flag is at the base
        FlagTaken, ///< The flag is taken. / A player is carrying a flag.
        FlagDropped ///< The flag is dropped.
    };

	///Flag Class, every flag should have this class, contains info about a flag
	/// Flag class with status and team id
	class Flag 
    {
    private:		
		irr::core::vector3df* m_StartPosition;
		irr::core::vector3df* m_StartRotation;
		EFlagEnum* m_FlagStatus;
        ETeamIdentifier* m_TeamIdentifier;
        irr::scene::IMeshSceneNode* m_FlagNode;
        Collider* m_Collider;
        irr::scene::ISceneNode* m_FlagOldParent = nullptr;
        TeamScore m_TeamScore;

    public: 
        /// <summary> Flag class constructor. </summary>
        /// <param name="a_Device">The active Irrlicht Device.</param>
        /// <param name="a_TeamIdentifier">The team's identifier the flag should have.</param>
        /// <param name="a_TeamScore">Reference to the teamscore class.</param>
        Flag(irr::IrrlichtDevice* a_Device, ETeamIdentifier a_TeamIdentifier, TeamScore a_TeamScore);
        /// <summary> Flag class destructor </summary>
        ~Flag();
		/// <summary> Capture Flag a flag with the wanted playerobject parent. </summary>
		/// <param name="a_PlayerObject"> The player object that should carry the flag. </param>
		void captureFlag(Player* a_PlayerObject);
		/// <summary> Set the starting position of the flag that it will reset to. </summary>
		/// <param name="a_Position"> The position the flag will reset to. </param>
        void setStartPosition(irr::core::vector3df a_Position);
		/// <summary> Set the starting rotation of the flag that it will reset to. </summary>
		/// <param name="a_Rotation"> The rotation the flag will reset to. </param>
        void setStartRotation(irr::core::vector3df a_Rotation);
		/// <summary> Will drop the flag at the current position. </summary>
		/// <param name="a_PlayerObject"> The player object that drops the flag. </param>
		void drop(Player* a_PlayerObject);
		/// <summary> Will return the flag to it's starting position and rotation. </summary>
		void returnToStartPosition();
		/// <summary> The player carrying a flag has gotten a point. </summary>
		/// <param name="a_PlayerObject"> Score a point for the player object. </param>
		void score(Player* a_PlayerObject);
		/// <summary> Set the collision of the level and players and add an physics animation. </summary>
		/// <param name="a_SceneManager"> Pass the scenemanager to add a physics animator. </param>
		/// <param name="a_TriangleSelector"> The triangle seletor that has the level and players. </param>
        void setCollisionTriangleSelector(irr::scene::ISceneManager* a_SceneManager, irr::scene::ITriangleSelector* a_TriangleSelector);
    private:
        void initParticleSystem(irr::scene::ISceneManager* a_SceneManager);
		void setColor(irr::video::IVideoDriver* a_VideoDriver);
		irr::video::SColor getColor();
	};
}
