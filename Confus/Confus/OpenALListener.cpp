#include "OpenALListener.h";

OpenALListener::OpenALListener() {
	init();
}

void OpenALListener::init() {
	ALFWInit();
	if (!ALFWInitOpenAL())
	{
		ALFWShutdown();
		throw; //Failed to init
	}
}

void OpenALListener::clear() {
	ALFWShutdownOpenAL();
	ALFWShutdown();
}


void OpenALListener::setPosition(float a_PositionX, float a_PositionY, float a_PositionZ) {
	init();
	alListener3f(AL_POSITION, a_PositionX, a_PositionY, a_PositionZ);
}

void OpenALListener::setDirection(float a_AtX, float a_AtY, float a_AtZ, float a_UpX, float a_UpY, float a_UpZ) {
	init();

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
void OpenALListener::setVelocity(float a_VelocityX, float a_VelocityY, float a_VelocityZ) {
	init();

	alListener3f( AL_VELOCITY, a_VelocityX, a_VelocityY, a_VelocityZ);
}

OpenALListener::~OpenALListener() {
	clear();
}