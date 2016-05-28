#pragma once
#include <Irrlicht/irrlicht.h>

namespace Confus
{	
	/// <summary>
	/// Performs FPS camera controls for a given camera object and delegates the direction to another instance
	/// </summary>
	/// <remarks>
	/// Note that this takes full control of the target at the camera, which cannot be changed (easily) without it being
	/// overriden by this controller. You should be able to instead change the rotation of the camera, as it is used
	/// to derive the old target
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
		/// <summary>The node to control the XZ/Y direction of</summary>
		irr::scene::ISceneNode* m_DirectionNode;		
		/// <summary>
		/// Whether we wrapped the rotation in the last frame, so that we can prevent 
		/// the algorithm from detecting a wrap, warping the position, as a movement of the mouse
		/// </summary>
		bool m_Wrapped = true;		
		/// <summary>
		/// The rotation around the x-axis, stored so that we do not have to derive this from the
		/// target vector of the camera
		/// </summary>
		float m_XRotation = 0.0f;
		/// <summary>The maximum rotation around the X axis, so that we cannot look over 360 degrees, which is unnatural</summary>
		float m_MaximumXRotation = 60.0f;		
		/// <summary>The minimum rotation around the X axis, so that we cannot look over 360 degrees, which is unnatural</summary>
		float m_MinimumXRotation = -60.0f;
		/// <summary>
		/// The sensitivity of the mouse, where the units correspond to the movement from 
		/// one window boundary to the other on their respective axis
		/// </summary>
		irr::core::vector2df m_MouseSensitivity = irr::core::vector2df(100.0f, 100.0f);		
		/// <summary>
		/// The multiplier for the axes, so that we can invert them at our desire
		/// and decouple this multiplier from the mouse sensitivity
		/// </summary>
		irr::core::vector2df m_AxesMultiplier = irr::core::vector2df(-1.0f, -1.0f);

	public:		
		/// <summary>Initializes a new instance of the <see cref="CameraController"/> class.</summary>
		/// <param name="a_Device">The active Irrlicht device</param>
		/// <param name="a_CameraNode">The camera node to control</param>
		/// <param name="a_DirectionNode">The node to control the XZ/Y direction of</param>
		CameraController(irr::IrrlichtDevice* a_Device, irr::scene::ICameraSceneNode* a_CameraNode,
			irr::scene::ISceneNode* a_DirectionNode);
		
		/// <summary>Updates the target vector of the camera and direction of the attached node</summary>
		void update();
	private:		
		/// <summary>Wraps the mouse within the boundaries of the screen</summary>
		void wrapMouse();
		
		/// <summary>Updates the target of the camera in a way that it will be pointing in the direction of the mouse</summary>
		void updateTarget();
	};
}