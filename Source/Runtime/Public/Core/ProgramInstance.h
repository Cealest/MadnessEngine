/* Copyright 2015 Myles Salholm */
#pragma once

#include "Runtime/Public/Definitions.h"
#include "Runtime/Public/Core/Window.h"
#include "Runtime/Public/Core/Array.h"

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

	/* Function which creates a game in the specified window. */
	class FGameInstance* AddGameInstance(FWindow* InWindow);

#if WINDOWS
	/* The main program execution loop when running on Windows. */
	int ProgramExecutionLoopWindows(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

	/* Function which creates a window not tied to a game instance. */
	FWindow* AddWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
#endif

private:
	/* An array of game instances running in the program. */
	TArray<FGameInstance> GameInstances;

	/* An array of windows running in the program. */
	TArray<FWindow> Windows;
};