#pragma once
namespace ConfusShared
{
#pragma pack(push, 1)	
	/// <summary> Represents the state of the keys pressed by the player </summary>
	struct PlayerInputState
	{
		/// <summary>Whether the move forward key is pressed</summary>
		bool ForwardPressed = false;
		/// <summary>Whether the move backward key is pressed</summary>
		bool BackwardPressed = false;
		/// <summary>whether the move left key is pressed</summary>
		bool LeftPressed = false;
		/// <summary>Whether the move right key is pressed</summary>
		bool RightPressed = false;
		/// <summary>Whether the right mouse key is pressed</summary>
		bool LeftMouseButtonPressed = false;
		/// <summary>Whether the left mouse key is pressed</summary>
		bool RightMouseButtonPressed = false;
	};
#pragma pack(pop)
}