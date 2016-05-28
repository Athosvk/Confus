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
		/// <summary>
		/// The rotation of the node, stored so that we can used it as a sequence of rotation, rather than derive
		/// the quaternion from the target vector of the camera
		/// </summary>
		irr::core::quaternion m_Rotation = irr::core::quaternion();
		/// <summary>The maximum rotation around the X axis, so that we cannot look over 360 degrees, which is unnatural</summary>
		float m_MaximumXRotation = 60.0f;		
		/// <summary>The minimum rotation around the X axis, so that we cannot look over 360 degrees, which is unnatural</summary>
		float m_MinimumXRotation = -60.0f;
		/// <summary>
		/// The sensitivity of the mouse, where one unit in radian correspond to the movement from 
		/// one window boundary to the other on their respective axis
		/// </summary>
		irr::core::vector2df m_MouseSensitivity = irr::core::vector2df(90.0f, 90.0f);
		/// <summary>
		/// The multiplier for the axes, so that we can invert them at our desire
		/// and decouple this multiplier from the mouse sensitivity
		/// </summary>
		irr::core::vector2df m_AxesMultiplier = irr::core::vector2df(-1.0f, -1.0f);		
		/// <summary>
		/// The Y rotation in euler angles, stored since the toEulerAngles method is clamped for Y rotation,
		/// so we manually set this
		/// </summary>
		float m_YRotation = 0.0f;

	public:		
		/// <summary>Initializes a new instance of the <see cref="CameraController"/> class.</summary>
		/// <param name="a_Device">The active Irrlicht device</param>
		/// <param name="a_CameraNode">The camera node to control</param>
		CameraController(irr::IrrlichtDevice* a_Device, irr::scene::ICameraSceneNode* a_CameraNode);
		
		/// <summary>Updates the target vector of the camera and direction of the attached node</summary>
		void update();
		
		/// <summary>Gets the y rotation of the camera in euler angles in degrees</summary>
		/// <returns></returns>
		float getYRotation() const;
	private:
		/// <summary>Wraps the mouse within the boundaries of the screen</summary>
		void wrapMouse();
		
		/// <summary>Updates the target of the camera in a way that it will be pointing in the direction of the mouse</summary>
		void updateTarget();
		
		/// <summary> Gives a quaternion derived from an axis and angle. Serves as helper method, since it is not available</summary>
		/// <param name="a_Axis">The axis the rotation is about</param>
		/// <param name="a_Angle">The angle in radians around the axis</param>
		/// <returns>The resulting quaternion</returns>
		irr::core::quaternion fromAxisAngle(irr::core::vector3df a_Axis, float a_Angle);
	};
}