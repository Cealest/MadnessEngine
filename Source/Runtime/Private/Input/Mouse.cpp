/* Copyright 2015 Myles Salholm */
#include "Runtime/Public/Input/Mouse.h"

FMouse::FMouse()
{
	Position.X = 0;
	Position.Y = 0;
	OldPosition.X = 0;
	OldPosition.Y = 0;
}

FMouse::~FMouse()
{

}

bool FMouse::Initialize()
{
	return true;
}

void FMouse::LeftButtonDown()
{
	bLeftMouseDown = true;
	LeftMouseDownEvent.Notify();
}

void FMouse::LeftButtonUp()
{
	bLeftMouseDown = false;
	LeftMouseUpEvent.Notify();
}

void FMouse::RightButtonDown()
{
	bRightMouseDown = true;
	RightMouseDownEvent.Notify();
}

void FMouse::RightButtonUp()
{
	bRightMouseDown = false;
	RightMouseUpEvent.Notify();
}

void FMouse::MouseMove(int InX, int InY)
{
	bool bMouseMoved = false;
	if (Position.X != InX || Position.Y != InY)
		bMouseMoved = true;
	OldPosition = Position;
	Position.X = (float)InX;
	Position.Y = (float)InY;
	if (bMouseMoved)
	{
		MoveEvent.Notify();
	}
}