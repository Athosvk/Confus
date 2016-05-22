#include "stdafx.h"

#include "../Confus/Flag.h"
#include "../ConfusShared/Physics/PhysicsWorld.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ConfusTest
{
    TEST_CLASS(FlagTest)
    {
        Confus::Physics::PhysicsWorld Physics;
        Confus::Flag* Flag;
        irr::IrrlichtDevice* Device;
        
    public:
        FlagTest() : Physics(Device)
        {}

        TEST_METHOD_INITIALIZE(setupFlag)
        {
            Device = irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_NULL);
            Flag = new Confus::Flag(Device, ETeamIdentifier::TeamBlue, Physics);
        }

        TEST_METHOD_CLEANUP(teardownFlag)
        {
            Device->drop();
            delete(Flag);
        }

        TEST_METHOD(testGetFlagColorAfterConstruction)
        {
            Assert::AreEqual(static_cast<char>(Flag->getTeamIdentifier()), static_cast<char>(ETeamIdentifier::TeamBlue));
        }
    };
}