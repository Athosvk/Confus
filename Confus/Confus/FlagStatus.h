#pragma once
#include <Irrlicht\irrlicht.h>
namespace Confus
{
	class FlagStatus
	{
	private:
		irr::gui::IGUIEnvironment* m_GUIEnvironment;
		irr::video::IVideoDriver* m_Driver;
		irr::gui::IGUIImage* m_RedFlag;
		irr::gui::IGUIImage* m_BlueFlag;
		irr::gui::IGUIImage* m_RedExclamationMark;
		irr::gui::IGUIImage* m_BlueExclamationMark;
		irr::video::ITexture* m_FlagTexture;
		irr::video::ITexture* m_ExclamationMarkTexture;
	public:
		FlagStatus(irr::gui::IGUIEnvironment* a_GUIEnvironment, irr::video::IVideoDriver* a_Driver);
		~FlagStatus();
	};
}
