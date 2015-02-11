/* Copyright 2015 Myles Salholm */
#pragma once

#include "Runtime/Public/Definitions.h"
#include "Runtime/Public/Core/GameInstance.h"
#include "Runtime/Public/Core/Window.h"

/*
ProgramInstance represents a single instance of the program.  It acts as
a static global wrapper to abstractly interact with the target platform.
There should only be one ProgramInstance per program.
*/
//@TODO Make dynamic arrays for both the Window and GameInstance members.
class FProgramInstance
{
public:
	/* Why we're shutting down the program. 0 means everything went smoothly. */
	int ShutdownReason;

	/* Constructor. */
	FProgramInstance();
	/* Destructor. */
	~FProgramInstance();

#if WINDOWS
	bool ProgramExecutionLoopWindows(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

	/* Function which creates a window not tied to a game instance. */
	FWindow* AddWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
#endif

private:
	/* A non-GameInstance window. */
	FWindow* Window;

	/* The actual GameInstance. */
	//@TODO Actually make and manage a game instance.
	FGameInstance* GameInstance;
};