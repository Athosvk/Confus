#include "stdafx.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ConfusTest
{
	TEST_CLASS(PlayerTest)
	{
		irr::IrrlichtDevice* m_Device = irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_NULL);

		TEST_METHOD(testLightAttack)
		{
			ConfusShared::Physics::PhysicsWorld physicsWorld(m_Device);
			ConfusShared::Player player = ConfusShared::Player(m_Device, physicsWorld, 0);
			player.startLightAttack();
			Assert::AreEqual(player.getState(), ConfusShared::EPlayerState::LightAttacking);
		}

		TEST_METHOD(testHeavyAttack)
		{
			ConfusShared::Physics::PhysicsWorld physicsWorld(m_Device);
			ConfusShared::Player player = ConfusShared::Player(m_Device, physicsWorld, 0);
			player.startHeavyAttack();
			Assert::AreEqual(player.getState(), ConfusShared::EPlayerState::HeavyAttacking);
		}

		TEST_METHOD(testRespawn)
		{
			ConfusShared::Physics::PhysicsWorld physicsWorld(m_Device);
			ConfusShared::Player player = ConfusShared::Player(m_Device, physicsWorld, 0);
			const auto StartPosition = irr::core::vector3df(5.0f, 5.0f, 5.0f);
			player.setStartPosition(StartPosition);
			player.respawn();
			Assert::AreEqual(player.getPosition(), StartPosition);
		}

		TEST_METHOD(testTeamID)
		{
			ConfusShared::Physics::PhysicsWorld physicsWorld(m_Device);
			ConfusShared::Player player = ConfusShared::Player(m_Device, physicsWorld, 0);
			const auto TeamIdentifier = ConfusShared::ETeamIdentifier::TeamBlue;
			player.setTeamIdentifier(TeamIdentifier, m_Device);
			Assert::AreEqual(player.getTeamIdentifier(), TeamIdentifier);
		}

		TEST_METHOD(testDeathRespawn)
		{
			ConfusShared::Physics::PhysicsWorld physicsWorld(m_Device);
			ConfusShared::Player player = ConfusShared::Player(m_Device, physicsWorld, 0);
			const auto StartPosition = irr::core::vector3df(5.0f, 5.0f, 5.0f);
			player.setStartPosition(StartPosition);
			player.getHealthInstance()->damage(player.getHealthInstance()->getHealth(), EHitIdentifier::Light);
			player.update();
			Assert::AreEqual(player.getPosition(), StartPosition);
		}

		TEST_METHOD(testDrop)
		{
			ConfusShared::Physics::PhysicsWorld physicsWorld(m_Device);
			ConfusShared::Player player = ConfusShared::Player(m_Device, physicsWorld, 0);
			const auto StartPosition = irr::core::vector3df(5.0f, 5.0f, 5.0f);
			player.setStartPosition(StartPosition);

			ConfusShared::Flag flag = ConfusShared::Flag(m_Device, ConfusShared::ETeamIdentifier::None, physicsWorld);
			player.FlagPointer = &flag;
			player.getHealthInstance()->damage(player.getHealthInstance()->getHealth(), EHitIdentifier::Light);
			player.update();
			Assert::IsTrue(player.FlagPointer == nullptr);
		}

		TEST_METHOD(testPlayerStateReset)
		{
			ConfusShared::Physics::PhysicsWorld physicsWorld(m_Device);
			ConfusShared::Player player = ConfusShared::Player(m_Device, physicsWorld, 0);
			const auto StartPosition = irr::core::vector3df(5.0f, 5.0f, 5.0f);
			player.setStartPosition(StartPosition);

			ConfusShared::Flag flag = ConfusShared::Flag(m_Device, ConfusShared::ETeamIdentifier::None, physicsWorld);
			player.FlagPointer = &flag;
			player.getHealthInstance()->damage(player.getHealthInstance()->getHealth(), EHitIdentifier::Light);
			player.update();
			Assert::AreEqual(player.CarryingFlag, ConfusShared::EFlagEnum::None);
		}

		TEST_METHOD(testFlagStateReset)
		{
			ConfusShared::Physics::PhysicsWorld physicsWorld(m_Device);
			ConfusShared::Player player = ConfusShared::Player(m_Device, physicsWorld, 0);
			const auto StartPosition = irr::core::vector3df(5.0f, 5.0f, 5.0f);
			player.setStartPosition(StartPosition);

			ConfusShared::Flag flag = ConfusShared::Flag(m_Device, ConfusShared::ETeamIdentifier::None, physicsWorld);
			player.FlagPointer = &flag;
			player.getHealthInstance()->damage(player.getHealthInstance()->getHealth(), EHitIdentifier::Light);
			player.update();
			Assert::AreEqual(flag.getFlagStatus(), ConfusShared::EFlagEnum::FlagDropped);
		}
	};
}