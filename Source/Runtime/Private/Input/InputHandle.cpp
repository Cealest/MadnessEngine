/* Copyright 2015 Myles Salholm */
#include "Runtime/Public/Input/InputHandle.h"

FInputHandle::FInputHandle()
{
	
}

FInputHandle::~FInputHandle()
{

}

bool FInputHandle::Init()
{
	if (!Mouse.Initialize())
		return false;
	return true;
}

void FInputHandle::KeyDown(unsigned int Key)
{
	if (Key <= KEYS)
	{
		KeyStates[Key].Press();
	}
}

void FInputHandle::KeyUp(unsigned int Key)
{
	if (Key <= KEYS)
	{
		KeyStates[Key].Release();
	}
}

bool FInputHandle::IsKeyDown(unsigned int Key)
{
	if (Key <= KEYS)
	{
		return KeyStates[Key].IsPressed();
	}

	// That key doesn't exist.
	return false;
}

void FInputHandle::SubscribeToKeyPress(FObserver &InObserver, unsigned int Key)
{
	if (Key <= KEYS)
	{
		KeyStates[Key].PressedSubject.AddObserver(&InObserver);
	}
}

void FInputHandle::SubscribeToKeyRelease(FObserver &InObserver, unsigned int Key)
{
	if (Key <= KEYS)
	{
		KeyStates[Key].ReleasedSubject.AddObserver(&InObserver);
	}
}

void FInputHandle::SubscribeToLeftMouse(FObserver &InObserver, bool IsDown)
{
	if (IsDown)
		Mouse.LeftMouseDownEvent.AddObserver(&InObserver);
	else
		Mouse.LeftMouseUpEvent.AddObserver(&InObserver);
}

void FInputHandle::SubscribeToRightMouse(FObserver &InObserver, bool IsDown)
{
	if (IsDown)
		Mouse.RightMouseDownEvent.AddObserver(&InObserver);
	else
		Mouse.RightMouseUpEvent.AddObserver(&InObserver);
}

void FInputHandle::SubscribeToMouseMove(FObserver &InObserver)
{
	Mouse.MoveEvent.AddObserver(&InObserver);
}