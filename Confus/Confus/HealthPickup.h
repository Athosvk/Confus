#pragma once
#include <Irrlicht\irrlicht.h>

namespace Confus
{
	class Player;
	namespace Physics
	{
		class BoxCollider;
		class PhysicsWorld;
	}

	/// Flag Class, every flag should have this class, contains info about a flag
	/// Flag class with status and team id
	class HealthPickup
	{
	private:
		irr::core::vector3df m_StartPosition = irr::core::vector3df(0.f);
		irr::core::vector3df m_StartRotation = irr::core::vector3df(0.f);
		irr::scene::IMeshSceneNode* m_HealthPickupNode;
		Physics::BoxCollider* m_Collider;
		irr::scene::IAnimatedMesh* m_MeshNode;
		int m_HealAmount = 100;
		bool m_IsActive = true;
		double m_Timer = 0.0f;
		

	public:
		/// <summary> Flag class constructor </summary>
		/// <param name="a_Device">The active Irrlicht Device.</param>
		/// <param name="a_TeamIdentifier">The team's identifier the flag should have.</param>
		/// <param name="a_PhysicsWorld">The physics world </param>
		HealthPickup(irr::IrrlichtDevice* a_Device, Physics::PhysicsWorld& a_PhysicsWorld);

		/// <summary> Flag class destructor </summary>
		~HealthPickup();

		/// <summary> Set the starting position of the flag that it will reset to. </summary>
		/// <param name="a_Position"> The position the flag will reset to. </param>
		void setPosition(irr::core::vector3df a_Position);

		const irr::video::SColor getColor() const;

		void update(double a_DeltaTime);

	private:
		void initParticleSystem(irr::scene::ISceneManager* a_SceneManager);
		void setColor(irr::video::IVideoDriver* a_VideoDriver);
	};
}
