#pragma once
#include <irrlicht/irrlicht.h>

class Player
{
public:
    Player(irr::scene::ISceneManager * sceneManager);
    void fixedUpdate();
    void update();
};

