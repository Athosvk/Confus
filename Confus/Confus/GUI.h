#pragma once
#include <Irrlicht/irrlicht.h>
#include <memory>
#include <vector>

#include "../ConfusShared/Player.h"
#include "IUIElement.h"
#include "Audio/Sound.h"

namespace Confus
{
	/// <summary>
	/// The GUI representing the various graphical user interface objects in the game
	/// </summary>
	class GUI
	{
	private:
		/// <summary>The active player node</summary>
		ConfusShared::Player* m_PlayerNode;
		/// <summary>The active Irrlicht device</summary>
		irr::IrrlichtDevice* m_Device;
		/// <summary>The GUI environment used to draw the images</summary>
		irr::gui::IGUIEnvironment* m_GUIEnvironment;
		/// <summary>The active Irrlicht video driver</summary>
		irr::video::IVideoDriver* m_Driver;
		std::vector<std::unique_ptr<IUIElement>> m_Elements;
		/// <summary>The sound to play once the player gets low on health</summary>
		Audio::Sound m_AudioSourceLowHealth;
		
		
	public:
		/// <summary>Initializes a new instance of the <see cref="GUI"/> class.</summary>
		/// <param name="a_Device">The active Irrlicht device</param>
		/// <param name="a_Player">The active player instance</param>
		/// <param name="a_AudioManager">The audio manager to create the sounds on</param>
		GUI(irr::IrrlichtDevice* a_Device, ConfusShared::Player* a_Player, Audio::AudioManager* a_AudioManager);
		
		/// <summary>Finalizes an instance of the <see cref="GUI"/> class.</summary>
		~GUI();
		
		/// <summary>Updates this instance.</summary>
		void update();
		void lowHealthAudio();

		/// <summary>
		/// Creates and adds a GUI element of given type, which will be drawn on the screen and updated
		/// by the GUI. This allows for having the lifetime of the gui elements managed 
		/// by the GUI class itself
		/// </summary>
		template<typename TElementType, typename... TArguments>
		void addElement(TArguments&&... a_Arguments)
		{
			m_Elements.push_back(std::make_unique<TElementType>(std::forward<TArguments>(a_Arguments)...));
		}
		
		/// <summary>Calculates the relative pixel position.</summary>
		/// <param name="a_Coordinates">The coordinates in world space</param>
		/// <param name="a_CurrentResolution">The current resolution</param>
		/// <returns>The relative position of the object on the screen</returns>
		static irr::core::position2d<int> calculatePixelPosition(irr::core::vector2df a_Coordinates, irr::core::dimension2d<irr::u32> a_CurrentResolution);
	};
}
