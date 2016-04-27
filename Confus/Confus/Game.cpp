#include <Irrlicht/irrlicht.h>
#include <time.h>
#include <iostream>

#include "Game.h"
#include "Player.h"
#include "Flag.h"
#include "FlagGUI.h"
#include "ScoreGUI.h"
#define DEBUG_CONSOLE
#include "Debug.h"

namespace Confus
{
    const double Game::FixedUpdateInterval = 0.02;
    const double Game::MaxFixedUpdateInterval = 0.1;
	const int Game::MaxScore = 3; // dont change this for now. Breaks redside score.

    Game::Game(irr::core::dimension2d<irr::u32> a_Resolution)
        : m_Device(irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_OPENGL,a_Resolution)),
		m_MazeGenerator(m_Device,60,60, irr::core::vector3df(0.0f, 0.0f, 0.0f),(19+20+21+22+23+24), irr::core::vector2df(30.,30.)), // magic number is just so everytime the first maze is generated it looks the same, not a specific number is chosen
        m_PlayerNode(m_Device, 1, ETeamIdentifier::TeamBlue, true),
        m_SecondPlayerNode(m_Device, 1, ETeamIdentifier::TeamRed, false),
        m_BlueFlag(m_Device, ETeamIdentifier::TeamBlue),
        m_RedFlag(m_Device, ETeamIdentifier::TeamRed),
        m_RedRespawnFloor(m_Device),
        m_BlueRespawnFloor(m_Device),
		m_GUI(m_Device, &m_PlayerNode)
    {
		auto videoDriver = m_Device->getVideoDriver();
		m_GUI.addElement<FlagGUI>(m_Device, &m_BlueFlag, irr::core::dimension2du(50, 50),
					videoDriver->getTexture("Media/Textures/FlagUIImage.png"),
					videoDriver->getTexture("Media/Textures/ExclamationMark.png"),
					irr::core::vector2df(0.44f, 0.0f), false);

		m_GUI.addElement<FlagGUI>(m_Device, &m_RedFlag, irr::core::dimension2du(50, 50),
			videoDriver->getTexture("Media/Textures/MirroredFlagUIImage.png"),
			videoDriver->getTexture("Media/Textures/MirroredExclamationMark.png"),
			irr::core::vector2df(0.56f, 0.0f), true);

		m_GUI.addElement<ScoreGUI>(m_Device, &m_RedFlag, irr::core::dimension2du(30, 30),
			videoDriver->getTexture("Media/Textures/Orb.png"), irr::core::vector2df(0.59f, 0.061f));

		m_GUI.addElement<ScoreGUI>(m_Device, &m_BlueFlag, irr::core::dimension2du(30, 30),
			videoDriver->getTexture("Media/Textures/Orb.png"), irr::core::vector2df(0.45f, 0.061f));
    }

    void Game::run()
    {
        initializeConnection();
        auto sceneManager = m_Device->getSceneManager();
        m_LevelRootNode = m_Device->getSceneManager()->addEmptySceneNode();

        m_LevelRootNode->setPosition(irr::core::vector3df(1.0f, 1.0f, 1.0f));
        sceneManager->loadScene("Media/IrrlichtScenes/Bases 2.irr", nullptr, m_LevelRootNode);
        m_LevelRootNode->setScale(irr::core::vector3df(1.0f, 1.0f, 1.0f));
        m_LevelRootNode->setVisible(true);
        
        processTriangleSelectors();

        m_PlayerNode.setLevelCollider(m_Device->getSceneManager(), m_LevelRootNode->getTriangleSelector());
        m_SecondPlayerNode.setLevelCollider(m_Device->getSceneManager(), m_LevelRootNode->getTriangleSelector());
        m_BlueFlag.setCollisionTriangleSelector(m_Device->getSceneManager(), m_LevelRootNode->getTriangleSelector());
        m_RedFlag.setCollisionTriangleSelector(m_Device->getSceneManager(), m_LevelRootNode->getTriangleSelector());

        m_BlueRespawnFloor.setPosition(irr::core::vector3df(0.f, 3.45f, 11.f));
        m_RedRespawnFloor.setPosition(irr::core::vector3df(0.f, 3.45f, -83.f));

        m_Device->setEventReceiver(&m_EventManager);
        m_Device->getCursorControl()->setVisible(false);
      
        while(m_Device->run())
        {
            m_Connection->processPackets();
			handleInput();
            update();
            processFixedUpdates();
            render();
        }
    }

    void Game::processTriangleSelectors()
    {
        auto sceneManager = m_Device->getSceneManager();
        auto metatriangleSelector = sceneManager->createMetaTriangleSelector();
        
        irr::core::array<irr::scene::ISceneNode*> nodes;
        sceneManager->getSceneNodesFromType(irr::scene::ESNT_ANY, nodes);
        for(irr::u32 i = 0; i < nodes.size(); ++i)
        {
            irr::scene::ISceneNode* node = nodes[i];
            irr::scene::ITriangleSelector* selector = nullptr;

            switch(node->getType())
            {
            case irr::scene::ESNT_CUBE:
            case irr::scene::ESNT_ANIMATED_MESH:
                selector = m_Device->getSceneManager()->createTriangleSelectorFromBoundingBox(node);
                break;
            case irr::scene::ESNT_MESH:
            case irr::scene::ESNT_SPHERE:
                selector = sceneManager->createTriangleSelector(((irr::scene::IMeshSceneNode*)node)->getMesh(), node);
                break;
            case irr::scene::ESNT_TERRAIN:
                selector = sceneManager->createTerrainTriangleSelector((irr::scene::ITerrainSceneNode*)node);
                break;
            case irr::scene::ESNT_OCTREE:
                selector = sceneManager->createOctreeTriangleSelector(((irr::scene::IMeshSceneNode*)node)->getMesh(), node);
                break;
            default:
                break;
            }
            
            if(selector)
            {
                metatriangleSelector->addTriangleSelector(selector);
                selector->drop();
            }
        }
        m_LevelRootNode->setTriangleSelector(metatriangleSelector);
    }

    void Game::initializeConnection()
    {
        std::string serverIP;
        std::cout << "Enter the server's ip address: ";
        std::cin >> serverIP;

        unsigned short serverPort;
        std::cout << "Enter the server's port: ";
        std::cin >> serverPort;

        m_Connection = std::make_unique<Networking::ClientConnection>(serverIP, serverPort);
    }

    void Game::handleInput()
    {
        m_PlayerNode.handleInput(m_EventManager);
    }

    void Game::update()
    {
        m_PreviousTicks = m_CurrentTicks;
        m_CurrentTicks = m_Device->getTimer()->getTime();
        m_DeltaTime = (m_CurrentTicks - m_PreviousTicks) / 1000.0;

        m_PlayerNode.update();
		m_GUI.update();
        m_Listener.setPosition(m_PlayerNode.CameraNode->getAbsolutePosition());
        irr::core::quaternion playerRotation(m_PlayerNode.CameraNode->getRotation());

        //Todo: Fix rotations
        irr::core::vector3df upVector = playerRotation * irr::core::vector3df( 0, 1, 0 );
        irr::core::vector3df forwardVector = playerRotation * irr::core::vector3df(0, 0, 1);
        m_Listener.setDirection(forwardVector, upVector);     
    }

    void Game::processFixedUpdates()
    {
        m_FixedUpdateTimer += m_DeltaTime;
        m_FixedUpdateTimer = irr::core::min_(m_FixedUpdateTimer, MaxFixedUpdateInterval);
        while(m_FixedUpdateTimer >= FixedUpdateInterval)
        {
            m_FixedUpdateTimer -= FixedUpdateInterval;
            fixedUpdate();
        }
    }

    void Game::updateOtherPlayers(/*array a_PlayerInfo*/)
    {
        /*
        for(int i = 0; i < a_PlayerInfo.size(); i++)
        {
            for(int j = 0; j < m_PlayerArray.size(); j++)
            {
                if(m_PlayerArray[j].MainPlayer == false && m_PlayerArray[j].ID == a_PlayerInfo[i].ID)
                {
                    m_PlayerArray[j].setPosition(a_PlayerInfo[i].position);
                    m_PlayerArray[j].setRotation(a_PlayerInfo[i].rotation); 
                    break;
                }
            }
        }
        */
    }

    void Game::fixedUpdate()
    {
		static float timer = 0.0f;
		timer += static_cast<float>(m_DeltaTime);
        if(timer >= 3.0f && timer <= 8.0f)
        {
            m_BlueRespawnFloor.enableCollision();
            m_RedRespawnFloor.enableCollision();
        }
		if (timer >= 9.0f)
		{
			timer = 0.0f;
			m_MazeGenerator.refillMainMaze(static_cast<int>(time(0)));
            m_BlueRespawnFloor.disableCollision();
            m_RedRespawnFloor.disableCollision();
		}
		m_MazeGenerator.fixedUpdate();
    }

    void Game::render()
    {
        m_Device->getVideoDriver()->beginScene(true, true, irr::video::SColor(255, 100, 101, 140));
        m_Device->getSceneManager()->drawAll();
        m_Device->getGUIEnvironment()->drawAll();
        m_Device->getVideoDriver()->endScene();
    }
}