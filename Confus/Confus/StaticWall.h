#pragma once
#include <Irrlicht/irrlicht.h>

using namespace irr;

class StaticWall
{
private:
    StaticWall();

public:
    StaticWall(IrrlichtDevice* a_Device, core::vector3d<float> a_Position, core::vector3d<float> a_Rotation);
    ~StaticWall();
   
};

