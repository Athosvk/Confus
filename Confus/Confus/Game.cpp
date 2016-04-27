#include <Irrlicht/irrlicht.h>
#include <time.h>
#include <iostream>

#include "Game.h"
#include "Player.h"
#include "Flag.h"
#include "FlagGUI.h"
#define DEBUG_CONSOLE
#include "Debug.h"
#include "../ConfusShared/Physics//BoxCollider.h"

namespace Confus
{
    const double Game::FixedUpdateInterval = 0.03;
    const double Game::MaxFixedUpdateInterval = 0.1;

    Game::Game(irr::core::dimension2d<irr::u32> a_Resolution)
        : m_Device(irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_DIRECT3D9, a_Resolution)),
		m_PhysicsWorld(m_Device),
		m_MazeGenerator(m_Device, 60, 60,(19+20+21+22+23+24),  // magic number is just so everytime the first maze is generated it looks the same, not a specific number is chosen
			irr::core::vector2df(30., 30.), m_PhysicsWorld),
        m_PlayerNode(m_Device, m_PhysicsWorld, 1, ETeamIdentifier::TeamBlue, true),
        m_SecondPlayerNode(m_Device, m_PhysicsWorld, 1, ETeamIdentifier::TeamRed, false),
        m_BlueFlag(m_Device, ETeamIdentifier::TeamBlue, m_PhysicsWorld),
        m_RedFlag(m_Device, ETeamIdentifier::TeamRed, m_PhysicsWorld),
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

		updateSceneTransformations();
        initializeLevelColliders();

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

	void Game::initializeLevelColliders()
	{
		irr::core::array<irr::scene::ISceneNode*> nodes;
		m_Device->getSceneManager()->getSceneNodesFromType(irr::scene::ESNT_ANY, nodes, m_LevelRootNode);
		for(irr::u32 i = 0; i < nodes.size(); ++i)
		{
			irr::scene::ISceneNode* node = nodes[i];
			Physics::ICollider* collider = nullptr;

			if(node->isVisible())
			{
				switch(node->getType())
				{
				case irr::scene::ESNT_CUBE:
				case irr::scene::ESNT_ANIMATED_MESH:
				case irr::scene::ESNT_MESH:
					if(std::string(static_cast<const char*>(node->getName())).find("Ground", 0) != std::string::npos)
					{
						collider = m_PhysicsWorld.createBoxCollider(irr::core::vector3df(node->getBoundingBox().getExtent().X * 
							node->getScale().X, 0.3f, node->getBoundingBox().getExtent().Z * node->getScale().Z), node,
							Physics::ECollisionFilter::LevelStatic, Physics::ECollisionFilter::Player);
					}
					else
					{
						collider = m_PhysicsWorld.createBoxCollider(node->getScale(), node, Physics::ECollisionFilter::LevelStatic, 
							Physics::ECollisionFilter::Player);
					}
					collider->getRigidBody()->makeStatic();
					break;
				case irr::scene::ESNT_SPHERE:
				case irr::scene::ESNT_TERRAIN:
				case irr::scene::ESNT_OCTREE:
					break;
				default:
					break;
				}
			}
		}
	}

    void Game::initializeConnection()
    {
        //std::string serverIP;
        //std::cout << "Enter the server's ip address: ";
        //std::cin >> serverIP;

        //unsigned short serverPort;
        //std::cout << "Enter the server's port: ";
        //std::cin >> serverPort;

        m_Connection = std::make_unique<Networking::ClientConnection>("1", 1u);
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

		static float timer = 0.0f;
		timer += static_cast<float>(m_DeltaTime);
		if(timer >= 3.0f && timer <= 8.0f)
		{
			m_BlueRespawnFloor.enableCollision();
			m_RedRespawnFloor.enableCollision();
		}
		if(timer >= 100.0f)
		{
			timer = 0.0f;
			m_MazeGenerator.refillMainMaze(static_cast<int>(time(0)));
			m_BlueRespawnFloor.disableCollision();
			m_RedRespawnFloor.disableCollision();
		}
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

	void Game::updateSceneTransformations()
	{
		std::function<void(irr::scene::ISceneNode* a_Node)> updateDownwards = [&updateDownwards](irr::scene::ISceneNode* a_Node)
		{
			a_Node->updateAbsolutePosition();
			auto children = a_Node->getChildren();
			for(auto child : children)
			{
				updateDownwards(child);
			}
		};
		updateDownwards(m_Device->getSceneManager()->getRootSceneNode());
	}

    void Game::fixedUpdate()
    {
		m_MazeGenerator.fixedUpdate();
		m_PhysicsWorld.stepSimulation(static_cast<float>(FixedUpdateInterval));
    }

    void Game::render()
    {
        m_Device->getVideoDriver()->beginScene(true, true, irr::video::SColor(255, 100, 101, 140));
        m_Device->getSceneManager()->drawAll();
        m_Device->getGUIEnvironment()->drawAll();
		m_PhysicsWorld.drawDebugInformation();
        m_Device->getVideoDriver()->endScene();
    }
}