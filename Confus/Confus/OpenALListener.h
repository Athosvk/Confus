#pragma once
#include "framework/Win32/Framework.h"

class OpenALListener {
private:
	static void clear();
	static void init();
public:
	static void setPosition(float a_PositionX = 0.0f, float a_PositionY = 0.0f, float a_PositionZ = 0.0f);
	static void setVelocity(float a_VelocityX = 0.0f, float a_VelocityY = 0.0f, float a_VelocityZ = 0.0f);
	/// <summary> Set the direction of the sound listener.
	/// At is the at rotation vector (forward) and Up is the up rotation vector.
	/// </summary>
	static void setDirection(float a_AtX = 0.0f, float a_AtY = 0.0f, float a_AtZ = 0.0f, float a_UpX = 0.0f, float a_UpY = 0.0f, float a_UpZ = 0.0f);
	OpenALListener();
	~OpenALListener();
};