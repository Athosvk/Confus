#pragma once
namespace irr
{
    class IrrlichtDevice;
}

class Game
{
private:
    irr::IrrlichtDevice* m_Device;

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

