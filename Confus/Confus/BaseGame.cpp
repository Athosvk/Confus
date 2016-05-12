#include "BaseGame.h"

namespace Confus
{
    const double BaseGame::FixedUpdateInterval = 0.02;
    const double BaseGame::MaxFixedUpdateInterval = 0.1;

    BaseGame::BaseGame(irr::IrrlichtDevice* a_Device, EventManager* a_EventManager)
    {
        m_Device = a_Device;
        m_EventManager = a_EventManager;
        m_Device->setEventReceiver(m_EventManager);
    }

    void BaseGame::run()
    {
        start();
        m_CurrentTicks = m_Device->getTimer()->getTime();
        while(m_Device->run() && m_ShouldRun)
        {
            update();
            processFixedUpdates();
            render();
            m_PreviousTicks = m_CurrentTicks;
            m_CurrentTicks = m_Device->getTimer()->getTime();
            m_DeltaTime = (m_CurrentTicks - m_PreviousTicks) / 1000.0;
        }
        end();
    }

    void BaseGame::processFixedUpdates()
    {
        m_FixedUpdateTimer += m_DeltaTime;
        m_FixedUpdateTimer = irr::core::min_(m_FixedUpdateTimer, MaxFixedUpdateInterval);
        while(m_FixedUpdateTimer >= FixedUpdateInterval)
        {
            m_FixedUpdateTimer -= FixedUpdateInterval;
            fixedUpdate();
        }
    }

    void BaseGame::render()
    {
        m_Device->getVideoDriver()->beginScene(true, true, irr::video::SColor(255, 100, 101, 140));
        m_Device->getSceneManager()->drawAll();
        m_Device->getGUIEnvironment()->drawAll();
        m_Device->getVideoDriver()->endScene();
    }
}
