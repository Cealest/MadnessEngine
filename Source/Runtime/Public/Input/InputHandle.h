/* Copyright 2015 Myles Salholm */

#pragma once

#include "Runtime/Public/Definitions.h"

class FInputHandle
{
public:
	/* Constructor. */
	FInputHandle();
	/* Destructor. */
	~FInputHandle();

	/* Initializes the handle and its input states. */
	void Init();

	/* Sets key state to down. */
	void KeyDown(unsigned int Key);

	/* Sets key state to up. */
	void KeyUp(unsigned int Key);

	/* Returns true if the key is pressed down. */
	bool IsKeyDown(unsigned int Key);

private:
	/* Array of key states. */
	bool KeyStates[KEYS];
};