#pragma once
#include <Irrlicht\irrlicht.h>
#include "IUIElement.h"
#include "Flag.h"

class ScoreGUI : public IUIElement
{
private:
	irr::gui::IGUIImage* m_ScoreImage;
	irr::video::ITexture* m_ScoreTexture;
	irr::core::dimension2du m_ImageMaxSize;
public:
	ScoreGUI(irr::IrrlichtDevice* a_Device, Flag* a_Flag, irr::core::dimension2du a_ImageMaxSize, irr::video::ITexture* a_FlagTexture, irr::video::ITexture* a_ExclamationMarkTexuture, irr::core::vector2df a_RelativePosition);
	virtual void update() override;
	irr::core::dimension2du calculateOffset(irr::video::ITexture * a_Texture);
	~ScoreGUI();
};

