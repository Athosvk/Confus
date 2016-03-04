#pragma once
#include <Irrlicht/irrlicht.h>

class StaticWall
{
public:
    StaticWall(irr::IrrlichtDevice* a_Device, irr::core::vector3d<float> a_Position, irr::core::vector3d<float> a_Rotation);
    ~StaticWall();
};

