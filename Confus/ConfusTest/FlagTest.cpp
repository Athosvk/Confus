#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ConfusTest
{
	TEST_CLASS(FlagTest)
	{
		ConfusShared::Physics::PhysicsWorld m_PhysicsWorld;
		irr::IrrlichtDevice* m_Device = irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_NULL);
		ConfusShared::Flag* m_Flag;
		ConfusShared::Player m_Player;
	
	public:
		FlagTest() 
			: m_PhysicsWorld(m_Device),
			m_Player(m_Device, m_PhysicsWorld, 0)
		{
		}

		TEST_METHOD_INITIALIZE(setupFlag)
		{
			m_Flag = new ConfusShared::Flag(m_Device, ConfusShared::ETeamIdentifier::TeamBlue, m_PhysicsWorld);
		}

		TEST_METHOD_CLEANUP(teardownFlag)
		{
			m_Device->drop();
			delete(m_Flag);
		}

		TEST_METHOD(testInitialFlagColor) 
		{
			Assert::AreEqual(static_cast<char>(m_Flag->getTeamIdentifier()), static_cast<char>(ConfusShared::ETeamIdentifier::TeamBlue));
		}

		TEST_METHOD(testInitialFlagState) 
		{
			Assert::AreEqual(static_cast<char>(m_Flag->getFlagStatus()), static_cast<char>(ConfusShared::EFlagEnum::FlagBase));
		}

		TEST_METHOD(testReturnToStart)
		{
			irr::core::vector3df position(8.0f, 3.0f, 2.14f);
			m_Flag->setStartPosition(position);
			m_Flag->returnToStartPosition();
			Assert::AreEqual(m_Flag->getPosition(), position);
		}

		TEST_METHOD(testStatusChangeCallback)
		{
			ConfusShared::Flag flag(m_Device, ConfusShared::ETeamIdentifier::TeamBlue, m_PhysicsWorld);
			bool succeeded = false;
			flag.FlagStatusChangedEvent += [&succeeded](ConfusShared::ETeamIdentifier a_TeamIdentifier, ConfusShared::EFlagEnum a_PreviousState,
				ConfusShared::EFlagEnum a_CurrentState) 
			{
				succeeded = true;
			};
			flag.captureFlag(&m_Player);
			Assert::IsTrue(succeeded);
		}

		TEST_METHOD(testStatusChangeIdentifier)
		{
			auto teamIdentifier = ConfusShared::ETeamIdentifier::TeamBlue;
			ConfusShared::Flag flag(m_Device, ConfusShared::ETeamIdentifier::TeamBlue, m_PhysicsWorld);
			bool succeeded = false;
			flag.FlagStatusChangedEvent += [&succeeded, teamIdentifier](ConfusShared::ETeamIdentifier a_TeamIdentifier, 
				ConfusShared::EFlagEnum a_PreviousState, ConfusShared::EFlagEnum a_CurrentState)
			{
				succeeded = a_TeamIdentifier == teamIdentifier;
			};
			flag.captureFlag(&m_Player);
			Assert::IsTrue(succeeded);
		}

		TEST_METHOD(testCapture)
		{
			ConfusShared::Flag flag(m_Device, ConfusShared::ETeamIdentifier::TeamBlue, m_PhysicsWorld);
			m_Player.setTeamIdentifier(ConfusShared::ETeamIdentifier::TeamRed, m_Device);
			bool succeeded = false;
			flag.FlagStatusChangedEvent += [&succeeded](ConfusShared::ETeamIdentifier a_TeamIdentifier, ConfusShared::EFlagEnum a_PreviousState,
				ConfusShared::EFlagEnum a_CurrentState)
			{
				succeeded = a_PreviousState == ConfusShared::EFlagEnum::FlagBase && a_CurrentState == ConfusShared::EFlagEnum::FlagTaken;
			};
			flag.captureFlag(&m_Player);
			Assert::IsTrue(succeeded);
		}

		TEST_METHOD(testCaptureFail)
		{
			auto teamIdentifier = ConfusShared::ETeamIdentifier::TeamRed;
			ConfusShared::Flag flag(m_Device, teamIdentifier, m_PhysicsWorld);
			m_Player.setTeamIdentifier(teamIdentifier, m_Device);
			bool succeeded = true;
			flag.FlagStatusChangedEvent += [&succeeded](ConfusShared::ETeamIdentifier a_TeamIdentifier, ConfusShared::EFlagEnum a_PreviousState,
				ConfusShared::EFlagEnum a_CurrentState)
			{
				succeeded = false;
			};
			flag.captureFlag(&m_Player);
			Assert::IsTrue(succeeded);
		}

		TEST_METHOD(testDropState)
		{
			ConfusShared::Flag flag(m_Device, ConfusShared::ETeamIdentifier::TeamRed, m_PhysicsWorld);
			bool succeeded = false;
			m_Player.setTeamIdentifier(ConfusShared::ETeamIdentifier::TeamBlue, m_Device);
			flag.captureFlag(&m_Player);

			flag.FlagStatusChangedEvent += [&succeeded](ConfusShared::ETeamIdentifier a_TeamIdentifier, ConfusShared::EFlagEnum a_PreviousState,
				ConfusShared::EFlagEnum a_CurrentState)
			{
				succeeded = a_PreviousState == ConfusShared::EFlagEnum::FlagTaken && a_CurrentState == ConfusShared::EFlagEnum::FlagDropped;
			};
			flag.drop(irr::core::vector3df());
			Assert::IsTrue(succeeded);
		}

		TEST_METHOD(testDropPosition)
		{
			ConfusShared::Flag flag(m_Device, ConfusShared::ETeamIdentifier::TeamRed, m_PhysicsWorld);
			auto dropPosition = irr::core::vector3df(0.0f, 10.0f, 19.9f);
			flag.drop(dropPosition);
			Assert::AreEqual(dropPosition, flag.getPosition());
		}
	};
}
