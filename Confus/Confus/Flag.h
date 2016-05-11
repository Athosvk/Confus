#pragma once
#include "Networking\ClientConnection.h"
#include "../ConfusShared/TeamIdentifier.h"
#include <Irrlicht\irrlicht.h>
#include "Delegate.h"

namespace Confus 
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

	///Flag Class, every flag should have this class, contains info about a flag
	/// Flag class with status and team id
	class Flag 
    {
	public:
		Delegate<void(ETeamIdentifier a_TeamIdentifier, EFlagEnum a_PreviousFlagEnum, EFlagEnum a_CurrentFlagEnum)> FlagStatusChangedEvent;
    private:		
		irr::core::vector3df m_StartPosition = irr::core::vector3df(0.f);
		irr::core::vector3df m_StartRotation = irr::core::vector3df(0.f);
        ETeamIdentifier m_TeamIdentifier;
		EFlagEnum m_FlagStatus;
        irr::scene::IMeshSceneNode* m_FlagNode;
        Physics::BoxCollider* m_Collider;
        irr::scene::ISceneNode* m_FlagOldParent = nullptr;


        /// <summary> A pointer to the connection with the server. </summary>
        Networking::ClientConnection* m_Connection;
    public: 
		/// <summary> Flag class constructor </summary>
		/// <param name="a_Device">The active Irrlicht Device.</param>
		/// <param name="a_TeamIdentifier">The team's identifier the flag should have.</param>
		/// <param name="a_PhysicsWorld">The physics world </param>

        void setConnection(Networking::ClientConnection * a_Connection);
		Flag(irr::IrrlichtDevice* a_Device, ETeamIdentifier a_TeamIdentifier,
			Physics::PhysicsWorld& a_PhysicsWorld);

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

        /// <summary> Sets the position of the flagnode. </summary>
        /// <param name="a_Rotation"> The new position to set the flag to. </param>
        void setPosition(irr::core::vector3df a_Position);

		/// <summary> Will drop the flag at the current position. </summary>
		/// <param name="a_PlayerObject"> The player object that drops the flag. </param>
		void drop(Player* a_PlayerObject);

		/// <summary> Will return the flag to it's starting position and rotation. </summary>
		void returnToStartPosition();

		/// <summary> The player carrying a flag has gotten a point. </summary>
		/// <param name="a_PlayerObject"> Score a point for the player object. </param>
		void score(Player* a_PlayerObject);

		const irr::video::SColor getColor() const;
		const ETeamIdentifier getTeamIdentifier() const;
		const EFlagEnum getFlagStatus() const;
        void setFlagStatus(EFlagEnum a_FlagStatus);
    private:
        void initParticleSystem(irr::scene::ISceneManager* a_SceneManager);
		void setColor(irr::video::IVideoDriver* a_VideoDriver);
	};
}
