#pragma once
namespace ConfusShared
{
#pragma pack(push, 1)	
	/// <summary> Represents the state of the keys pressed by the player </summary>
	struct PlayerInputState
	{
		bool ForwardPressed = false;
		bool BackwardPressed = false;
		bool LeftPressed = false;
		bool RightPressed = false;
	};
#pragma pack(pop)
}