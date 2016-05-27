#pragma once
#include <Irrlicht\irrlicht.h>
#include <vector>

#include "IUIElement.h"

#include "../ConfusShared/Player.h"

namespace Confus
{
	class HealthGUI : public IUIElement
	{
	private:
		irr::video::ITexture* m_HealthImage;
		irr::video::ITexture* m_HealthTexture;
		irr::core::dimension2du m_ImageMaxSize;
		std::vector<irr::gui::IGUIImage*> m_HealthImages;
		irr::video::ITexture* m_BloodTexture;
		std::vector<irr::gui::IGUIImage*> m_BloodImages;
		ConfusShared::Player* m_PlayerNode;
	public:
		HealthGUI(irr::IrrlichtDevice* a_Device, ConfusShared::Player* a_PlayerNode, irr::core::dimension2du a_ImageMaxSize, irr::video::ITexture* a_HealthTexture, irr::video::ITexture* a_BloodTexture, irr::core::vector2df a_RelativePosition);
		virtual void update() override;
		~HealthGUI();
	};
}

