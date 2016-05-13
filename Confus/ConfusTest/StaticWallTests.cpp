#include "stdafx.h"
#include <exception>
#include "CppUnitTest.h"
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

        TEST_METHOD(SetupTest)
        {
            Assert::IsNotNull(wall);
        }

        TEST_METHOD(SetPositionTest)
        {
            wall->setSceneNode(irr::core::vector3df(1.1f, 2.2f, 3.3f), irr::core::vector3df());
            irr::core::vector3df newPosition = wall->getPosition();
            Assert::AreEqual(newPosition.X, 1.1f);
        }

        TEST_METHOD(SetRotationTest)
        {
            wall->setSceneNode(irr::core::vector3df(), irr::core::vector3df(10.1f, 200.2f, 165.3f));
            irr::core::vector3df newRotation = wall->getRotation();
            Assert::AreEqual(newRotation.Z, 165.3f);
        }

        TEST_METHOD_CLEANUP(DeleteStaticWall)
        {
            delete(wall);
            camera->drop();
        }
     };
}