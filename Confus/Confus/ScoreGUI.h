#pragma once
#include <Irrlicht\irrlicht.h>
#include <vector>

#include "../ConfusShared/Flag.h"
#include "IUIElement.h"

namespace Confus
{
	class ClientTeamScore;
	
	/// <summary>
	/// Represents the GUI element that presents the score on the screen for each team
	/// </summary>
	/// <seealso cref="IUIElement" />
	class ScoreGUI : public IUIElement
	{
	private:		
		/// <summary>The texture to use for the score images</summary>
		irr::video::ITexture* m_ScoreTexture;		
		/// <summary>The maximum size of the score texture</summary>
		irr::core::dimension2du m_ImageMaxSize;		
		/// <summary>The images on the screen for representing the currrent score</summary>
		std::vector<irr::gui::IGUIImage*> m_ScoreImages;		
		/// <summary>The flag of the given team</summary>
		ConfusShared::Flag* m_Flag;		
		/// <summary>The current team score, so that the state of the score images can be based of it</summary>
		ClientTeamScore& m_TeamScore;
	public:		
		/// <summary>Initializes a new instance of the <see cref="ScoreGUI"/> class.</summary>
		/// <param name="a_Device">The active Irrlicht device</param>
		/// <param name="a_Flag">The flag of the given team to represent the score of</param>
		/// <param name="a_ImageMaxSize">Maximum size of image to show for the score</param>
		/// <param name="a_ScoreTexture">The texture to use for representing the score objects</param>
		/// <param name="a_RelativePosition">The position relative to the center of the screen, so that they can be offset accordingly</param>
		/// <param name="a_TeamScore">The score of the team</param>
		ScoreGUI(irr::IrrlichtDevice* a_Device, ConfusShared::Flag* a_Flag, irr::core::dimension2du a_ImageMaxSize,
			irr::video::ITexture* a_ScoreTexture, irr::core::vector2df a_RelativePosition, ClientTeamScore& a_TeamScore);

		/// <summary>Updates this instance.</summary>
		virtual void update() override;
		
		/// <summary>Finalizes an instance of the <see cref="ScoreGUI"/> class.</summary>
		~ScoreGUI();
	};
}

