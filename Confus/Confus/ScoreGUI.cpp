#include "ScoreGUI.h"
#include "GUI.h"
#include "Game.h"
#include "ClientTeamScore.h"

namespace Confus
{
	//TODO Add TeamScoreManager 
	ScoreGUI::ScoreGUI(irr::IrrlichtDevice * a_Device, ConfusShared::Flag * a_Flag, irr::core::dimension2du a_ImageMaxSize, 
		irr::video::ITexture * a_ScoreTexture, irr::core::vector2df a_RelativePosition)
		:m_ImageMaxSize(a_ImageMaxSize), m_Flag(a_Flag)
	{
		irr::core::dimension2du offset = calculateOffset(m_ImageMaxSize, a_ScoreTexture);
		auto guiEnvironment = a_Device->getGUIEnvironment();
		auto videoDriver = a_Device->getVideoDriver();
		
		for (int i = 0; i < Game::MaxScore; i++)
		{
			m_ScoreImages.push_back(addImage(guiEnvironment, GUI::calculatePixelPosition(a_RelativePosition, a_Device->getVideoDriver()->getScreenSize()) - irr::core::dimension2di(offset.Width / 2 + i*offset.Width, 0),
				a_ScoreTexture, irr::video::SColor(255,255,255,255), m_ImageMaxSize));
		}
	}

	void ScoreGUI::update()
	{
		int score = ClientTeamScore::getTeamScore(m_Flag->getTeamIdentifier());
		for (int i = 0; i < Game::MaxScore; i++)
		{
			if (score > i)
				m_ScoreImages[i]->setColor(m_Flag->getColor());
		}
	}

	ScoreGUI::~ScoreGUI()
	{
	}
}
