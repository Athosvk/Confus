#include "stdafx.h"

#include <Confus/Player.h>
#include <ConfusShared/Physics/PhysicsWorld.h>
#include <Confus/Audio/AudioManager.h>

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ConfusTest
{
    TEST_CLASS(ElementInArrayTest)
    {
        irr::IrrlichtDevice* m_Device = irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_NULL);
        Confus::Audio::AudioManager m_AudioManager;
        Confus::Physics::PhysicsWorld m_PhysicsWorld;

        std::vector<Confus::Player*> m_PlayerArray;

    public:
        ElementInArrayTest()
            : m_PhysicsWorld(m_Device)
        {

        }

        TEST_METHOD(testFillArray)
        {
            for(size_t i = 0u; i < 5; i++)
            {
                Confus::Player* newPlayer = new Confus::Player(m_Device, m_PhysicsWorld, i*1000, ETeamIdentifier::TeamBlue, false, &m_AudioManager);
                m_PlayerArray.push_back(newPlayer);
            }
        }
        TEST_METHOD(testFindElement)
        {
            for(size_t i = 0u; i < m_PlayerArray.size(); i++)
            {
                if(m_PlayerArray[i]->ID == 3000)
                {
                    m_PlayerArray[i]->remove();
                    delete(m_PlayerArray[i]);
                    m_PlayerArray.erase(m_PlayerArray.begin() + i);
                    break;
                }
            }
            testPrintArray();
        }
        TEST_METHOD(testPrintArray)
        {
            for(size_t i = 0u; i < m_PlayerArray.size(); i++)
            {
                if(m_PlayerArray[i]->ID == 3000)
                {
                    Assert::Fail();
                }
            }
        }
    };
}