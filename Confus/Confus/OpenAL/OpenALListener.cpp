#include <stdexcept>

#include "OpenALListener.h";

namespace Confus {
    struct Vector3 {
        float x, y, z;
    };

    void OpenALListener::init()
    {
        OpenAL::ALFWInit();
        if(!OpenAL::ALFWInitOpenAL())
        {
            OpenAL::ALFWShutdown();
            throw std::invalid_argument("Failed to init");
        }
    }

    void OpenALListener::dispose()
    {
        OpenAL::ALFWShutdownOpenAL();
        OpenAL::ALFWShutdown();
    }


    void OpenALListener::setPosition(float a_PositionX, float a_PositionY, float a_PositionZ)
    {
        alListener3f(AL_POSITION, a_PositionX, a_PositionY, a_PositionZ);
    }

    void OpenALListener::setPosition(Vector3 a_Position) {
        setPosition(a_Position.x, a_Position.y, a_Position.z);
    }

    void OpenALListener::setDirection(float a_AtX, float a_AtY, float a_AtZ, float a_UpX, float a_UpY, float a_UpZ) 
    {
        ALfloat direction[6];
        //Forward Vector
        direction[0] = a_AtX;
        direction[1] = a_AtY;
        direction[2] = a_AtZ;

        //Up Vector
        direction[3] = a_UpX;
        direction[4] = a_UpY;
        direction[5] = a_UpZ;
        alListenerfv(AL_DIRECTION, direction);
    }

    void OpenALListener::setDirection(Vector3 a_ForwardVector, Vector3 a_UpVector) {
        setDirection(a_ForwardVector.x, a_ForwardVector.y, a_ForwardVector.z, a_UpVector.x, a_UpVector.y, a_UpVector.z);
    }

    void OpenALListener::setVelocity(float a_VelocityX, float a_VelocityY, float a_VelocityZ)
    {
        alListener3f(AL_VELOCITY, a_VelocityX, a_VelocityY, a_VelocityZ);
    }

    void OpenALListener::setVelocity(Vector3 a_Velocity) {
        setVelocity(a_Velocity.x, a_Velocity.y, a_Velocity.z);
    }
}