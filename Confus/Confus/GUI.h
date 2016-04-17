#pragma once
#include <Irrlicht/irrlicht.h>

#include "Player.h"
#include "OpenAL\OpenALSource.h"

namespace Confus
{
	class GUI
	{
	private:
		Player* m_PlayerNode;
		irr::gui::IGUIEnvironment* m_GUIEnvironment;
		irr::gui::IGUIStaticText* m_HealthTextBox;
		irr::core::stringw m_HealthString = L"";
		irr::video::IVideoDriver* m_Driver;
		irr::video::ITexture* m_BloodImage;
		irr::gui::IGUIImage* m_BloodOverlay;
		OpenALSource* m_AudioSourceLowHealth;
	public:
		GUI(irr::IrrlichtDevice* a_Device, Player* a_Player);
		~GUI();
		void update();
		void drawBloodOverlay();
		void lowHealthAudio();
		static irr::core::position2d<int> CalculatePixelPosition(irr::core::vector2df a_Coordinates);
	};
}
