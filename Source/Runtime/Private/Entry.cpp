/* Copyright 2015 Myles Salholm */
#include "Runtime/Public/Core/ProgramInstance.h"

/* The global program instance. */
static FProgramInstance GProgramInstance = FProgramInstance();

#if WINDOWS
/*
The actual entry point for Windows applications.
*/
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	// Initialize the program.
	if (GProgramInstance.AddWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow))
	{
		// Begin the main execution loop.
		while (GProgramInstance.ProgramExecutionLoopWindows(hInstance, hPrevInstance, lpCmdLine, nCmdShow))
		{
			// Wait for execution to stop.
		}
	}

	return GProgramInstance.ShutdownReason;
}
#endif