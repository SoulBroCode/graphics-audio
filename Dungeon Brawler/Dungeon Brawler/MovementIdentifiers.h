#ifndef MOVEMENTIDENTIFIERS_H
#define MOVEMENTIDENTIFIERS_H


namespace Movement
{
	enum class Direction
	{
		Left = 0,
		Right = 128 * 4,	//MAXIMUM FRAME
	};

	enum class JumpState
	{
		OnGround,
		JumpUp,
		InAir,
	};
}

#endif //MOVEMENTIDENTIFIERS_H