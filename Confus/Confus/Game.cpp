#include <Irrlicht/irrlicht.h>
#include <iostream>
#include <RakNet/BitStream.h>
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/GetTime.h>

#include "Game.h"
#include "../ConfusShared/Flag.h"
#include "FlagGUI.h"
#include "WinScreen.h"
#include "../ConfusShared/PacketType.h"
#include "ScoreGUI.h"
#define DEBUG_CONSOLE
#include "../ConfusShared/Debug.h"
#include "../ConfusShared/TeamIdentifier.h"
#include "../ConfusShared/Physics/BoxCollider.h"

namespace Confus
{
	const int Game::MaxScore = 3; // dont change this for now. Breaks redside score.

    Game::Game(irr::IrrlichtDevice* a_Device, ConfusShared::EventManager* a_EventManager) : BaseGame(a_Device, a_EventManager),
        m_PhysicsWorld(m_Device),
        m_MazeGenerator(m_Device, 41, 40, (19 + 20 + 21 + 22 + 23 + 24),  // magic number is just so everytime the first maze is generated it looks the same, not a specific number is chosen
	        irr::core::vector2df(19., 20.), m_PhysicsWorld),
		m_PlayerHandler(a_Device, m_PhysicsWorld, m_AudioManager),
        m_GUI(m_Device, m_PlayerHandler.getMainPlayer(), &m_AudioManager),
        m_BlueFlag(m_Device, ConfusShared::ETeamIdentifier::TeamBlue, m_PhysicsWorld),
        m_RedFlag(m_Device, ConfusShared::ETeamIdentifier::TeamRed, m_PhysicsWorld),
        m_Announcer(&m_RedFlag,&m_BlueFlag, m_PlayerHandler.getMainPlayer(), &m_AudioManager),
		m_RedRespawnFloor(m_Device, m_PhysicsWorld, irr::core::vector3df(0.f, 3.45f, 11.f)),
		m_BlueRespawnFloor(m_Device, m_PhysicsWorld, irr::core::vector3df(0.f, 3.45f, -83.f)),
		m_MazeChangedSound(m_AudioManager.createSound("Wall rising.wav")),
		m_LevelRootNode(m_Device->getSceneManager()->addEmptySceneNode())
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
			videoDriver->getTexture("Media/Textures/Orb.png"), irr::core::vector2df(0.59f, 0.061f), m_ClientScore);

		m_GUI.addElement<ScoreGUI>(m_Device, &m_BlueFlag, irr::core::dimension2du(30, 30),
			videoDriver->getTexture("Media/Textures/Orb.png"), irr::core::vector2df(0.45f, 0.061f), m_ClientScore);
		
		m_MazeChangedSound.setVolume(0.2f);
		m_MazeGenerator.addMazeChangedListener([this]()
		{
			m_MazeChangedSound.play();
		});
		irr::scene::ICameraSceneNode* camera = m_Device->getSceneManager()->addCameraSceneNodeFPS(m_Device->getSceneManager()->getRootSceneNode());
		camera->setFOV(70.f);
		camera->setNearValue(0.1f);
		camera->setPosition(irr::core::vector3df(0.f, 0.0f, 0.2f));
		camera->setParent(m_PlayerHandler.getMainPlayer());
    }

    Game::~Game()
    {
		m_Device->getGUIEnvironment()->clear();
        m_Device->getSceneManager()->clear();
    }

    void Game::start()
    {
        m_Device->setWindowCaption(L"Game");
        initializeConnection();
        m_LevelRootNode->setPosition(irr::core::vector3df(1.0f, 1.0f, 1.0f));
		m_Device->getSceneManager()->loadScene("Media/IrrlichtScenes/Bases2.irr", nullptr, m_LevelRootNode);
        m_LevelRootNode->setScale(irr::core::vector3df(1.0f, 1.0f, 1.0f));
        m_LevelRootNode->setVisible(true);
		updateSceneTransformations();
        initializeLevelColliders();
        m_Device->getCursorControl()->setVisible(false);
    }

	void Game::initializeLevelColliders()
	{
		irr::core::array<irr::scene::ISceneNode*> nodes;
		m_Device->getSceneManager()->getSceneNodesFromType(irr::scene::ESNT_ANY, nodes, m_LevelRootNode);
		for(irr::u32 i = 0; i < nodes.size(); ++i)
		{
			irr::scene::ISceneNode* node = nodes[i];
			ConfusShared::Physics::Collider* collider = nullptr;

			switch(node->getType())
			{
			case irr::scene::ESNT_CUBE:
			case irr::scene::ESNT_ANIMATED_MESH:
			case irr::scene::ESNT_MESH:
				if(std::string(node->getName()).find("Ground", 0) != std::string::npos)
				{
					collider = m_PhysicsWorld.createBoxCollider(node, ConfusShared::Physics::ECollisionFilter::LevelStatic,
						ConfusShared::Physics::ECollisionFilter::Player | ConfusShared::Physics::ECollisionFilter::Interactable);
				}
				else if (std::string(node->getName()).find("Basefolder", 0) == std::string::npos)
				{
					collider = m_PhysicsWorld.createBoxCollider(node->getScale(), node, ConfusShared::Physics::ECollisionFilter::LevelStatic | 
						ConfusShared::Physics::ECollisionFilter::Interactable,
						ConfusShared::Physics::ECollisionFilter::Player);
                }
				if(collider != nullptr)
				{
					collider->getRigidBody()->makeStatic();
				}
				break;
			default:
				break;
			}
		}
	}

    void Game::initializeConnection()
    {
        std::string serverIP;
        std::cout << " loaded" << std::endl << "Enter the server's ip address: ";
        std::cin >> serverIP;

        unsigned short serverPort;
        std::cout << "Enter the server's port: ";
        std::cin >> serverPort;

        m_Connection = std::make_unique<Networking::ClientConnection>(serverIP, serverPort);
		m_PlayerHandler.setConnection(m_Connection.get());

        m_Connection->addFunctionToMap(static_cast<unsigned char>(ConfusShared::Networking::EPacketType::MazeChange), [this](RakNet::Packet* a_Data)
        {
            RakNet::BitStream bitstreamIn(a_Data->data, a_Data->length, false);

            int timeMazeChanges, mazeSeed;
            bitstreamIn.IgnoreBytes(sizeof(RakNet::MessageID));
            bitstreamIn.Read(timeMazeChanges);
            bitstreamIn.Read(mazeSeed);
            std::cout << "Update is in " << (timeMazeChanges - static_cast<int>(RakNet::GetTimeMS())) << " ms, the seed is:\t" << mazeSeed << std::endl;
            m_MazeGenerator.refillMainMazeRequest(mazeSeed, timeMazeChanges);
        });

        m_Connection->addFunctionToMap(static_cast<unsigned char>(ConfusShared::Networking::EPacketType::ScoreUpdate), [this](RakNet::Packet* a_Data)
        {
            RakNet::BitStream bitstreamIn(a_Data->data, a_Data->length, false);

            int redScore, blueScore;
            
            bitstreamIn.IgnoreBytes(sizeof(RakNet::MessageID));
            bitstreamIn.Read(redScore);
            bitstreamIn.Read(blueScore);
            m_ClientScore.setTeamScore(ConfusShared::ETeamIdentifier::TeamRed, redScore);
            m_ClientScore.setTeamScore(ConfusShared::ETeamIdentifier::TeamBlue, blueScore);
        });

        m_Connection->addFunctionToMap(static_cast<unsigned char>(ConfusShared::Networking::EPacketType::EndOfGame), [this](RakNet::Packet* a_Packet)
        {
            ConfusShared::ETeamIdentifier a_TeamIdentifier;
            RakNet::BitStream inputStream(a_Packet->data, a_Packet->length, false);
            inputStream.IgnoreBytes(sizeof(RakNet::MessageID));
            inputStream.Read(a_TeamIdentifier);

            WinScreen winScreen(m_Device, a_TeamIdentifier, m_EventManager);
            winScreen.run();
            m_Device->setWindowCaption(L"Game");

            if(winScreen.ShouldRestart)
            {
                reset();
            }
            else
            {
                m_ShouldRun = false;
            }
        });

		m_Connection->addFunctionToMap(ID_NO_FREE_INCOMING_CONNECTIONS, [this](RakNet::Packet* a_Data)
		{
			denyConnection(a_Data);
		});

		m_Connection->addFunctionToMap(ID_CONNECTION_ATTEMPT_FAILED, [this](RakNet::Packet* a_Data)
		{
			denyConnection(a_Data);
		});

		m_Connection->addFunctionToMap(ID_CONNECTION_LOST, [this](RakNet::Packet* a_Data)
		{
			denyConnection(a_Data);
		});
    }

    void Game::handleInput()
    {
        if(m_EventManager->IsKeyDown(irr::KEY_ESCAPE))
        {
            m_ShouldRun = false;
        }
		m_PlayerHandler.handleInput(m_EventManager);
    }

    void Game::update()
    {
        m_Connection->processPackets();
        handleInput();
		m_GUI.update();
		m_PlayerHandler.update();
        m_Listener.setPosition(m_PlayerHandler.getMainPlayer()->getAbsolutePosition());
        irr::core::matrix4 playerRotation(m_PlayerHandler.getMainPlayer()->getAbsoluteTransformation());
        irr::core::vector3df forwardVector = irr::core::vector3df(playerRotation[8], playerRotation[9], playerRotation[10] );
        irr::core::vector3df upVector = irr::core::vector3df(playerRotation[4], playerRotation[5], playerRotation[6]);
        m_Listener.setDirection(forwardVector, upVector);    
    }

    void Game::reset()
    {
        m_ClientScore.setTeamScore(ConfusShared::ETeamIdentifier::TeamBlue, 0);
        m_ClientScore.setTeamScore(ConfusShared::ETeamIdentifier::TeamRed, 0);
    }

    void Game::fixedUpdate()
    {
		m_MazeGenerator.fixedUpdate();
		m_PhysicsWorld.stepSimulation(static_cast<float>(FixedUpdateInterval));
		m_PlayerHandler.fixedUpdate();
    }

    void Game::end()
    {
        m_BlueFlag.returnToStartPosition();
        m_RedFlag.returnToStartPosition();
		m_Device->getCursorControl()->setVisible(true);
    }    

	void Game::updateSceneTransformations() const
	{
		//Recurses downwwards
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

	void Game::denyConnection(RakNet::Packet* a_Data)
	{
		RakNet::BitStream inputStream(a_Data->data, a_Data->length, false);
		RakNet::MessageID messageID;

		inputStream.Read(messageID);

		switch (messageID)
		{
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				std::cout << "Server is full" << std::endl;
			default:
				std::cout << "Could not connect to the server" << std::endl;
		}
		m_ShouldRun = false;
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
