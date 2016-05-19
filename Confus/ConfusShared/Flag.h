#pragma once
#include <Irrlicht\irrlicht.h>

#include "../ConfusShared/TeamIdentifier.h"
#include "Delegate.h"

namespace ConfusShared
{
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
		Delegate<void(ConfusShared::ETeamIdentifier a_TeamIdentifier, EFlagEnum a_PreviousFlagEnum, EFlagEnum a_CurrentFlagEnum)> FlagStatusChangedEvent;
    private:		
		irr::core::vector3df m_StartPosition = irr::core::vector3df(0.f);
		irr::core::vector3df m_StartRotation = irr::core::vector3df(0.f);
        ConfusShared::ETeamIdentifier m_TeamIdentifier;
		EFlagEnum m_FlagStatus;
        irr::scene::IMeshSceneNode* m_FlagNode;
        Physics::BoxCollider* m_Collider;
        irr::scene::ISceneNode* m_FlagOldParent = nullptr;
		Delegate<void()> m_OnScore;

    public: 
		/// <summary> Flag class constructor </summary>
		/// <param name="a_Device">The active Irrlicht Device.</param>
		/// <param name="a_TeamIdentifier">The team's identifier the flag should have.</param>
		/// <param name="a_PhysicsWorld">The physics world </param>
		Flag(irr::IrrlichtDevice* a_Device, ConfusShared::ETeamIdentifier a_TeamIdentifier,
			Physics::PhysicsWorld& a_PhysicsWorld);

        /// <summary> Flag class destructor </summary>
        ~Flag();

		/// <summary> Capture Flag a flag with the wanted playerobject parent. </summary>
		/// <param name="a_PlayerObject"> The player object that should carry the flag. </param>
		void captureFlag(Confus::Player* a_PlayerObject);

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

		const irr::video::SColor getColor() const;
		const ConfusShared::ETeamIdentifier getTeamIdentifier() const;
		const EFlagEnum getFlagStatus() const;
		void addScoreCallback(std::function<void()> a_Callback);
    private:
		void setFlagStatus(EFlagEnum a_FlagStatus);
        void initParticleSystem(irr::scene::ISceneManager* a_SceneManager);
		void setColor(irr::video::IVideoDriver* a_VideoDriver);
	};
}
