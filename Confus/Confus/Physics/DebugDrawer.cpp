#include "DebugDrawer.h"

#include <iostream>

namespace Confus
{
	namespace Physics
	{
		DebugDrawer::DebugDrawer(irr::IrrlichtDevice* a_Device, btCollisionWorld* a_World)
			: m_Device(a_Device),
			m_World(a_World)
		{
			m_World->setDebugDrawer(this);
		}

		void DebugDrawer::draw() const
		{
			irr::video::SMaterial debugMaterial;
			debugMaterial.Lighting = false;
			m_Device->getVideoDriver()->setMaterial(debugMaterial);
			m_Device->getVideoDriver()->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);
			m_World->debugDrawWorld();
		}

		void DebugDrawer::drawLine(const btVector3& a_From, const btVector3& a_To, const btVector3& a_Color)
		{
			irr::video::SColor color(255, static_cast<irr::u32>(a_Color[0] * 255.0), static_cast<irr::u32>(a_Color[1] * 255.0),
				static_cast<irr::u32>(a_Color[2] * 255.0));
			m_Device->getVideoDriver()->draw3DLine(irr::core::vector3df(a_From[0], a_From[1], a_From[2]),
				irr::core::vector3df(a_To[0], a_To[1], a_To[2]), color);
		}

		void DebugDrawer::drawContactPoint(const btVector3& a_Point, const btVector3& a_Normal, btScalar a_Distance,
			int a_LifeTime, const btVector3& a_Color)
		{
			static const irr::video::SColor Color(255, 255, 255, 0);

			const btVector3 to(a_Point + a_Normal * a_Distance);
			m_Device->getVideoDriver()->draw3DLine(irr::core::vector3df(a_Point[0], a_Point[1], a_Point[2]),
				irr::core::vector3df(to[0], to[1], to[2]), Color);
		}

		void DebugDrawer::setDebugMode(int a_Mode)
		{
			m_Mode = a_Mode;
		}

		int DebugDrawer::getDebugMode() const
		{
			return m_Mode;
		}

		void DebugDrawer::reportErrorWarning(const char* a_Warning)
		{
			std::cerr << a_Warning << std::endl;
		}

		void DebugDrawer::draw3dText(const btVector3& a_Location, const char* a_Text)
		{
			static int counter = 0;
			if(counter % 100 == 0)
			{
				std::cout << a_Text;
			}
			counter++;
			//Left empty on purpose. Do not need, but is abstract so needs implementation
		}
	}
}