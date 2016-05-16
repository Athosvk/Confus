#pragma once
#include <Irrlicht/irrlicht.h>
#include <memory>
#include <vector>

#include "Player.h"
#include "IUIElement.h"
#include "Audio\Sound.h"

namespace Confus
{
	class GUI
	{
	private:
		Player* m_PlayerNode;
		irr::IrrlichtDevice* m_Device;
		irr::gui::IGUIEnvironment* m_GUIEnvironment;
		irr::gui::IGUIStaticText* m_HealthTextBox;
		irr::core::stringw m_HealthString = L"";
		irr::video::IVideoDriver* m_Driver;
		irr::video::ITexture* m_BloodImage;
		irr::gui::IGUIImage* m_BloodOverlay;
		std::vector<std::unique_ptr<IUIElement>> m_Elements;
        Audio::Sound m_AudioSourceLowHealth;
		irr::video::ITexture* m_HealthImage;
		irr::gui::IGUIImage* m_HealthOne;
		irr::gui::IGUIImage* m_HealthTwo;
		irr::gui::IGUIImage* m_HealthThree;
		irr::gui::IGUIImage* m_HealthFour;
		
	public:
		GUI(irr::IrrlichtDevice* a_Device, Player* a_Player, Audio::AudioManager* a_AudioManager);
		~GUI();
		void update();
		void drawBloodOverlay();
		void lowHealthAudio();
		void drawHealth();
		template<typename TElementType, typename... TArguments>
		void addElement(TArguments&&... a_Arguments)
		{
			m_Elements.push_back(std::make_unique<TElementType>(std::forward<TArguments>(a_Arguments)...));
		}
		static irr::core::position2d<int> calculatePixelPosition(irr::core::vector2df a_Coordinates, irr::core::dimension2d<irr::u32> a_CurrentResolution);
	};
}
