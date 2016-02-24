#pragma once

#include <Irrlicht\irrlicht.h>
using namespace irr;

class Game
{
private:
    static const double FixedUpdateInterval;
    static const double MaxFixedUpdateInterval; 

    irr::IrrlichtDevice* m_Device;
    scene::ISceneManager* smgr;
    video::IVideoDriver* driver;

    double m_FixedUpdateTimer = 0.0;
    double m_DeltaTime = 0.0;

public:
    virtual ~Game() = default;
    void run();

private:
    void handleInput();
    void update();
    void processFixedUpdates();
    void fixedUpdate();
    void render();
};

