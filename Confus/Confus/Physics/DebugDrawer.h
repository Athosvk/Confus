#pragma once
#include <Bullet/btBulletCollisionCommon.h>
#include <Irrlicht/irrlicht.h>

namespace Confus
{
	namespace Physics
	{
		class DebugDrawer : public btIDebugDraw
		{
		private:
			irr::IrrlichtDevice* m_Device = nullptr;
			int m_Mode = 0;
			btCollisionWorld* m_World = nullptr;

		public:
			DebugDrawer(irr::IrrlichtDevice* a_Device, btCollisionWorld* a_World);

			void draw() const;
			virtual void drawLine(const btVector3& a_From, const btVector3& a_To, const btVector3& a_Color) override;
			virtual void drawContactPoint(const btVector3& a_Point, const btVector3& a_Normal, btScalar a_Distance,
				int a_LifeTime, const btVector3& a_Color) override;
			virtual void setDebugMode(int a_Mode) override;
			virtual int getDebugMode() const override;
			virtual void reportErrorWarning(const char* a_Warning);
			virtual void draw3dText(const btVector3& a_Location, const char* a_Text);
		};
	}
}
