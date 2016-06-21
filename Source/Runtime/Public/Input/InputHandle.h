/* Copyright 2015 Myles Salholm */
#pragma once

#include "Runtime/Public/Definitions.h"
#include "Runtime/Public/Templates/Array.h"
#include "Runtime/Public/Patterns/Observer.h"
#include "Runtime/Public/Input/Mouse.h"

class FKeyState
{
public:
	FKeyState()
	{
		bPressed = false;
	}
	void Press() { PressedSubject.Notify(); bPressed = true; }
	void Release() { ReleasedSubject.Notify(); bPressed = false; }
	bool IsPressed() const { return bPressed; }

	FSubject PressedSubject;
	FSubject ReleasedSubject;

private:
	bool bPressed;
};

/*
Handles the user's input.
*/
class FInputHandle
{
public:
	/* Constructor. */
	FInputHandle();
	/* Destructor. */
	~FInputHandle();

	/* Initializes the handle and its input states. */
	bool Init();

	/* Sets key state to down. */
	void KeyDown(unsigned int Key);

	/* Sets key state to up. */
	void KeyUp(unsigned int Key);

	/* Returns true if the key is pressed down. */
	bool IsKeyDown(unsigned int Key);

	/* Subscribes to be notified of key presses. */
	void SubscribeToKeyPress(FObserver &InObserver, unsigned int Key);
	/* Subscribes to be notified of key releases. */
	void SubscribeToKeyRelease(FObserver &InObserver, unsigned int Key);

	void SubscribeToLeftMouse(FObserver &InObserver, bool IsDown);
	void SubscribeToRightMouse(FObserver &InObserver, bool IsDown);
	void SubscribeToMouseMove(FObserver &InObserver);

	FMouse& GetMouse() { return Mouse; }

private:
	/* Array of key states. */
	FKeyState KeyStates[KEYS];
	FMouse Mouse;
};