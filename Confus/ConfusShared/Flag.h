#pragma once
#include <Irrlicht/irrlicht.h>

#include "../ConfusShared/TeamIdentifier.h"
#include "Delegate.h"

namespace ConfusShared
{
	class Player;
	namespace Physics
	{
		class BoxCollider;
		class PhysicsWorld;
	}

	/// <summary> The Flag's status. A player can have flagtaken or no flag. A flag can be taken, dropped or at base. </summary>
	enum class EFlagEnum
	{ 
		None, ///< Player is carrying nothing		  
		FlagBase, ///< Flag is at the base
		FlagTaken, ///< The flag is taken. / A player is carrying a flag.
		FlagDropped ///< The flag is dropped.
	};

	/// Flag Class, every flag should have this class, contains info about a flag
	/// Flag class with status and team id
	class Flag 
    {
	public:		
		/// <summary>The event that is called once the state of the flag has changed</summary>
		Delegate<void(ETeamIdentifier a_TeamIdentifier, EFlagEnum a_PreviousFlagEnum, EFlagEnum a_CurrentFlagEnum)> FlagStatusChangedEvent;
    private:			
		/// <summary>The start position of the flag, to where it respawns as well, so it can be respawned at any point in the game</summary>
		irr::core::vector3df m_StartPosition = irr::core::vector3df(0.f);		
		/// <summary>The starting rotation</summary>
		irr::core::vector3df m_StartRotation = irr::core::vector3df(0.f);		
		/// <summary>The team this flag belongs to, so that we can determine who can pick this flag up, as well as determine its team color</summary>
		ConfusShared::ETeamIdentifier m_TeamIdentifier;		
		/// <summary>The current status of the flag, so that we can display it in the GUI</summary>
		EFlagEnum m_FlagStatus;		
		/// <summary>The meshnode that represents the flag visually</summary>
		irr::scene::IMeshSceneNode* m_FlagNode;		
		/// <summary>The collider associated with the flag, so that it can be turned off and on later in the game</summary>
		Physics::BoxCollider* m_Collider;		
		/// <summary>The previous parent of the flag, so that its state can be reset appropriately after being dropped</summary>
		irr::scene::ISceneNode* m_FlagOldParent = nullptr;		
		/// <summary>
		/// The event that is called once this flag has been used to score a point, so that sounds and other events can 
		/// be triggered externally 
		/// </summary>
		Delegate<void()> m_OnScore;

    public: 
		/// <summary> Flag class constructor </summary>
		/// <param name="a_Device">The active Irrlicht Device.</param>
		/// <param name="a_TeamIdentifier">The team's identifier the flag should have.</param>
		/// <param name="a_PhysicsWorld">The physics world </param>
		Flag(irr::IrrlichtDevice* a_Device, ETeamIdentifier a_TeamIdentifier,
			Physics::PhysicsWorld& a_PhysicsWorld);

		/// <summary> Capture Flag a flag with the wanted playerobject parent. </summary>
		/// <param name="a_PlayerObject"> The player object that should carry the flag. </param>
		void captureFlag(Player* a_PlayerObject);

		/// <summary> Set the starting position of the flag that it will reset to. </summary>
		/// <param name="a_Position"> The position the flag will reset to. </param>
        void setStartPosition(irr::core::vector3df a_Position);

		/// <summary> Set the starting rotation of the flag that it will reset to. </summary>
		/// <param name="a_Rotation"> The rotation the flag will reset to. </param>
        void setStartRotation(irr::core::vector3df a_Rotation);

		/// <summary>Will drop the flag at the current position.</summary>
		/// <param name="a_DropPosition">The position to drop it at</param>
		void drop(irr::core::vector3df a_DropPosition);

		/// <summary> Will return the flag to it's starting position and rotation. </summary>
		void returnToStartPosition();

		/// <summary> The player carrying a flag has gotten a point. </summary>
		void score();
		
		/// <summary>Gets the color of the flag, based on the team identifier</summary>
		/// <returns>The current flag color</returns>
		const irr::video::SColor getColor() const;
		
		/// <summary>Gets the team identifier of the team this flag belongs to</summary>
		/// <returns>The flag's team identifier</returns>
		const ConfusShared::ETeamIdentifier getTeamIdentifier() const;
		
		/// <summary>Gets the flag status.</summary>
		/// <returns>The current state of the flag</returns>
		const EFlagEnum getFlagStatus() const;
		
		/// <summary>Adds a callback that will be called once a team has scored a point</summary>
		/// <param name="a_Callback">The callback to add to the delegate</param>
		void addScoreCallback(std::function<void()> a_Callback);
		
		/// <summary>Gets the position of the flag in world space coordinates</summary>
		/// <returns>The position of the flag</returns>
		irr::core::vector3df getPosition() const;
    private:		
		/// <summary>Sets the flag status.</summary>
		/// <param name="a_FlagStatus">The new flag state</param>
		void setFlagStatus(EFlagEnum a_FlagStatus);
		
		/// <summary>Initializes the particle system associated with the flag</summary>
		/// <param name="a_SceneManager">The scene manager to place the particle system in</param>
		void initParticleSystem(irr::scene::ISceneManager* a_SceneManager);
		
		/// <summary>Sets the color </summary>
		/// <param name="a_VideoDriver">So that it matches the color of the team it is associated with</param>
		void setColor(irr::video::IVideoDriver* a_VideoDriver);
	};
}
