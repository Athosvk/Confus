#pragma once
#include <Irrlicht\irrlicht.h>
#include "IUIElement.h"
#include <vector>

#include "../ConfusShared/Flag.h"

namespace Confus
{
	class ScoreGUI : public IUIElement
	{
	private:
		irr::video::ITexture* m_ScoreTexture;
		irr::core::dimension2du m_ImageMaxSize;
		std::vector<irr::gui::IGUIImage*> m_ScoreImages;
		Flag* m_Flag;
	public:
		ScoreGUI(irr::IrrlichtDevice* a_Device, Flag* a_Flag, irr::core::dimension2du a_ImageMaxSize, irr::video::ITexture* a_ScoreTexture, irr::core::vector2df a_RelativePosition);
		virtual void update() override;
		~ScoreGUI();
	};
}

