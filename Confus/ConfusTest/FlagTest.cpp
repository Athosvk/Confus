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
        Confus::Physics::PhysicsWorld m_Physics;
        Confus::Flag* m_Flag;
        irr::IrrlichtDevice* m_Device;

    public:
        FlagTest() : m_Physics(m_Device)
        {}

        TEST_METHOD_INITIALIZE(setupFlag)
        {
            m_Device = irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_NULL);
            m_Flag = new Confus::Flag(m_Device, ETeamIdentifier::TeamBlue, m_Physics);
        }

        TEST_METHOD_CLEANUP(teardownFlag)
        {
            m_Device->drop();
            delete(m_Flag);
        }

        TEST_METHOD(testGetFlagColorAfterConstruction)
        {
            Assert::AreEqual(static_cast<char>(m_Flag->getTeamIdentifier()), static_cast<char>(ETeamIdentifier::TeamBlue));
        }

        TEST_METHOD(testFlagStatusAfterConstruction)
        {
            Assert::AreEqual(static_cast<char>(m_Flag->getFlagStatus()), static_cast<char>(Confus::EFlagEnum::FlagBase));
        }

        TEST_METHOD(testReturnToStartPosition)
        {
            irr::core::vector3df newPosition(8, 3, 2.14f);
            m_Flag->setStartPosition(newPosition);
            m_Flag->returnToStartPosition();
            Assert::AreEqual(m_Flag->getPosition(), newPosition);
        }
    };
}



