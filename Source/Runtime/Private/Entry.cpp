/* Copyright 2015 Myles Salholm */
#include "Runtime/Public/Core/ProgramInstance.h"

const int DefaultWidth = 1024;
const int DefaultHeight = 768;

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
	GProgramInstance.Init();

#if WITH_EDITOR
	// Create the primary window.
	if (GProgramInstance.AddWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow, DefaultWidth, DefaultHeight))
	{
		// Begin the main execution loop.
		while (GProgramInstance.ProgramExecutionLoopWindows(hInstance, hPrevInstance, lpCmdLine, nCmdShow))
		{
			// Wait for execution to stop.
		}
	}
#else
	// Initialize the game.
	if (GProgramInstance.AddGameInstance(GProgramInstance.AddWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow, DefaultWidth, DefaultHeight)))
	{
		// Begin the main execution loop.
		while (GProgramInstance.ProgramExecutionLoopWindows(hInstance, hPrevInstance, lpCmdLine, nCmdShow))
		{
			// Wait for execution to stop.
		}
	}
#endif
	// Cleanup the program on the way out.
	GProgramInstance.Cleanup();

	return GProgramInstance.ShutdownReason;
}
#endif