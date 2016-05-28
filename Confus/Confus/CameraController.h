#pragma once
#include <Irrlicht/irrlicht.h>

namespace Confus
{	
	/// <summary>
	/// Performs FPS camera controls for a given camera object and delegates the direction to another instance
	/// </summary>
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
	};
}