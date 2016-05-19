#include "stdafx.h"
#include <exception>

#include "../Confus/GUI.h"
#include "../Confus/CrosshairGUI.h"
#include "../Confus/Player.h"
#include "../Confus/Audio/AudioManager.h"
#include "../ConfusShared/Physics/PhysicsWorld.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ConfusTest
{
    TEST_CLASS(CrosshairGUITest)
    {
        irr::IrrlichtDevice* m_Device;
        irr::video::IVideoDriver* m_videoDriver;

        TEST_METHOD_INITIALIZE(setup)
        {
            m_Device = irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_NULL);
            m_videoDriver = m_Device->getVideoDriver();
        }

        TEST_METHOD_CLEANUP(teardown)
        {
            m_Device->drop();
        }

        TEST_METHOD(testBadFilepath)
        {
            Assert::ExpectException<std::exception>(&CrosshairGUITest::createCrosshair);
        }

        static void createCrosshair()
        {
            irr::IrrlichtDevice* device = irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_NULL);
            irr::video::IVideoDriver* videoDriver = device->getVideoDriver();

            Confus::CrosshairGUI* crosshair = new Confus::CrosshairGUI(device, irr::core::dimension2du(70, 70),
                    videoDriver->getTexture("Nonsense"), irr::core::vector2df(0.50f, 0.50f));
        }
    };
}