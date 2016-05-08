#include "stdafx.h"
#include <exception>
#include <Confus\StaticWall.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ConfusTest
{
    TEST_CLASS(StaticWallTests)
    {
        irr::IrrlichtDevice* device;
        irr::scene::ICameraSceneNode* camera;
        Confus::StaticWall* wall;

        TEST_METHOD_INITIALIZE(CreateStaticWall)
        {
            device = irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_OPENGL, irr::core::dimension2d<irr::u32>(1920, 1080));
            camera = device->getSceneManager()->addCameraSceneNodeFPS(0, 100.0f, 0.01f, 1, nullptr, 5, true, 0.5f, false, true);
            wall = new Confus::StaticWall(device, irr::core::vector3df(), irr::core::vector3df(), camera);
        }

        TEST_METHOD_CLEANUP(DeleteStaticWall)
        {
           delete(wall);
           camera->drop();
        }

        TEST_METHOD(SetupTest)
        {
            Assert::IsNotNull(wall);
        }

        TEST_METHOD(ExpectException)
        {
            auto func = [this] { throwException(); };
            Assert::ExpectException<int>(func);
        }

        void throwException()
        {
            throw 5;
        }
    };
}