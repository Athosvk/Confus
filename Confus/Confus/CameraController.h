#pragma once
#include <Irrlicht/irrlicht.h>

namespace Confus
{	
	/// <summary>
	/// Performs FPS camera controls for a given camera object and delegates the direction to another instance
	/// </summary>
	/// <remarks>
	/// Note that this takes full control of the target at the camera, which cannot be changed (easily) without it being
	/// overriden by this controller
	/// </remarks>
	class CameraController
	{
	private:
		/// <summary>The camera of which the camera should be controlled</summary>
		irr::scene::ICameraSceneNode* m_CameraNode;
		/// <summary>The active Irrlicht device to retrieve the cursor control and windowstate from</summary>
		irr::IrrlichtDevice* m_Device;
		/// <summary>The previous mouse position</summary>
		irr::core::vector2df m_PreviousPosition;
		/// <summary>
		/// Whether we wrapped the rotation in the last frame, so that we can prevent 
		/// the algorithm from detecting a wrap, warping the position, as a movement of the mouse
		/// </summary>
		bool m_Wrapped = true;		
		/// <summary>The maximum rotation around the X axis, so that we cannot look over 360 degrees, which is unnatural</summary>
		float m_MaximumXRotation = 60.0f;		
		/// <summary>The minimum rotation around the X axis, so that we cannot look over 360 degrees, which is unnatural</summary>
		float m_MinimumXRotation = -60.0f;
		/// <summary>
		/// The sensitivity of the mouse, where one unit in radian correspond to the movement from 
		/// one window boundary to the other on their respective axis
		/// </summary>
		irr::core::vector2df m_MouseSensitivity = irr::core::vector2df(200.0f, 100.0f);
		/// <summary>
		/// The multiplier for the axes, so that we can invert them at our desire
		/// and decouple this multiplier from the mouse sensitivity
		/// </summary>
		irr::core::vector2df m_AxesMultiplier = irr::core::vector2df(-1.0f, -1.0f);
		/// <summary>
		/// The pseudo mirror of the camera's rotation
		/// </summary>
		irr::core::vector3df m_Rotation = irr::core::vector3df();
		/// <summary>The node to orbit around</summary>
		irr::scene::ISceneNode* m_OrbitNode;
	public:		
		/// <summary>Initializes a new instance of the <see cref="CameraController" /> class.</summary>
		/// <param name="a_Device">The active Irrlicht device</param>
		/// <param name="a_CameraNode">The camera node to control</param>
		/// <param name="a_OrbitNode">The node to orbit around</param>
		CameraController(irr::IrrlichtDevice* a_Device, irr::scene::ICameraSceneNode* a_CameraNode,
			irr::scene::ISceneNode* a_OrbitNode);
		
		/// <summary>Updates the target vector of the camera and direction of the attached node</summary>
		void fixedUpdate();
		
		/// <summary>Gets the y rotation of the camera in euler angles in degrees</summary>
		/// <returns></returns>
		float getYRotation() const;
	private:
		/// <summary>Wraps the mouse within the boundaries of the screen</summary>
		void wrapMouse();
		
		/// <summary>Updates the rotation of the camera in a way that it will be pointing in the direction of the mouse</summary>
		void updateRotation();
		
		/// <summary>Updates the target to aim the camera at</summary>
		void updateTarget();
	};
}