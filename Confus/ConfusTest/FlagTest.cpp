#include "stdafx.h"

#include <string>
#include "../Confus/Flag.h"
#include "../ConfusShared/Physics/PhysicsWorld.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft 
{
    namespace VisualStudio 
    {
        namespace CppUnitTestFramework 
        {
            template<>
            static std::wstring ToString<irr::core::vector3df>(const irr::core::vector3df  & vector) 
            {
                return std::to_wstring(vector.X) + L" " + std::to_wstring(vector.Y) + L" " + std::to_wstring(vector.Z);
            }
        }
    }
}

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

        TEST_METHOD(testFlagStatusAfterConstruction)
        {
            Assert::AreEqual(static_cast<char>(Flag->getFlagStatus()), static_cast<char>(Confus::EFlagEnum::FlagBase));
        }

        TEST_METHOD(testReturnToStartPosition)
        {
            irr::core::vector3df newPosition(8, 3, 2.14f);
            Flag->setStartPosition(newPosition);
            Flag->returnToStartPosition();
            Assert::AreEqual(Flag->getPosition(), newPosition);
        }
    };
}



