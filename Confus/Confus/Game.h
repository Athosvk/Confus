#pragma once
namespace irr
{
    class IrrlichtDevice;
}

namespace Confus {
	class OpenALListener;
}



class Game
{
private:
    static const double FixedUpdateInterval;
    static const double MaxFixedUpdateInterval; 

    irr::IrrlichtDevice* m_Device;
	Confus::OpenALListener* m_Listener;
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

