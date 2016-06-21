/* Copyright 2015 Myles Salholm */
#pragma once

#include "Runtime/Public/Definitions.h"
#include "Runtime/Public/Patterns/Observer.h"

class FMouse
{
public:
	FMouse();
	~FMouse();

	virtual bool Initialize();
	virtual void LeftButtonDown();
	virtual void LeftButtonUp();
	virtual void RightButtonDown();
	virtual void RightButtonUp();
	virtual void MouseMove(int InX, int InY);

	Vector2D GetPosition() const { return Position; }
	Vector2D GetLastPosition() const { return OldPosition; }

	bool IsLeftMouseDown() const { return bLeftMouseDown; }
	bool IsRightMouseDown() const { return bRightMouseDown; }

private:
	friend class FInputHandle;

	Vector2D Position;
	Vector2D OldPosition;
	FSubject MoveEvent;
	FSubject LeftMouseDownEvent;
	FSubject LeftMouseUpEvent;
	FSubject RightMouseDownEvent;
	FSubject RightMouseUpEvent;

	bool bLeftMouseDown;
	bool bRightMouseDown;

};