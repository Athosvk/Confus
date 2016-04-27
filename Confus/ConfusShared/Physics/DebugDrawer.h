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
			/// <summary> The currently active Irrlicht Device </summary>
			irr::IrrlichtDevice* m_Device = nullptr;
			/// <summary> The current debug render mode </summary>
			int m_Mode = 0;
			/// <summary> The active Bullet world </summary>
			btCollisionWorld* m_World = nullptr;

		public:
			DebugDrawer(irr::IrrlichtDevice* a_Device, btCollisionWorld* a_World);
			
			/// <summary> Draws the entire world's debug infromation </summary>
			void draw() const;

			/// <summary> Draws a line between given points </summary>
			/// <param name="a_From"> The starting point </param>
			/// <param name="a_To"> The end point </param>
			/// <param name="a_Color"> The color </param>
			virtual void drawLine(const btVector3& a_From, const btVector3& a_To, const btVector3& a_Color) override;

			/// <summary> Draws the contact point </summary>
			/// <param name="a_Point"> The point of contact </param>
			/// <param name="a_Normal"> The contact normal </param>
			/// <param name="a_Distance"> The distance between the objects colliding </param>
			/// <param name="a_LifeTime"> Unused </param>
			/// <param name="a_Color> The color to draw with </param>
			virtual void drawContactPoint(const btVector3& a_Point, const btVector3& a_Normal, btScalar a_Distance,
				int a_LifeTime, const btVector3& a_Color) override;

			/// <summary> Sets the debug drawing mode </summary>
			/// <param name="a_Mode"> The new mdoe </param>
			virtual void setDebugMode(int a_Mode) override;

			/// <summary> Gets the debug mode </summary>
			/// <returns> The current debug drawing mode </returns>
			virtual int getDebugMode() const override;

			/// <summary> Displays or logs the given warning </summary>
			/// <param name="a_Warning"> The warning text </param>
			virtual void reportErrorWarning(const char* a_Warning);

			/// <summary> Draws the given text </summary>
			/// <param name="a_Location"> The draw location </param>
			/// <param name="a_Text"> The text to draw </param>
			virtual void draw3dText(const btVector3& a_Location, const char* a_Text);
		};
	}
}
