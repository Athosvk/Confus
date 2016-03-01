#pragma once
#include <irrlicht/irrlicht.h>

namespace Confus {
	class Player
	{
	public:
		Player(irr::scene::ISceneManager * sceneManager);
		void fixedUpdate();
		void update();
		enum TeamIdentifier m_TeamIdentifier;
		enum FlagEnum m_CarryingFlag;
	};
}



