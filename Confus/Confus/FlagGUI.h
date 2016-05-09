#pragma once
#include <Irrlicht\irrlicht.h>
#include <string>
#include "Flag.h"
#include "IUIElement.h"

namespace Confus
{
	class FlagGUI : public IUIElement
	{
	private:
		Flag* m_Flag;
		irr::gui::IGUIImage* m_FlagImage;
		irr::gui::IGUIImage* m_ExclamationMarkImage;
		irr::video::ITexture* m_FlagTexture;
		irr::video::ITexture* m_ExclamationMarkTexture;
		irr::core::dimension2du m_ImageMaxSize;

	public:
		FlagGUI(irr::IrrlichtDevice* a_Device, Flag* a_Flag, irr::core::dimension2du a_ImageMaxSize, irr::video::ITexture* a_FlagTexture, irr::video::ITexture* a_ExclamationMarkTexuture, irr::core::vector2df a_RelativePosition, bool a_LeftSide);
		void changeFlagState();
		virtual void update() override;
		~FlagGUI();
	};
}

