/* Copyright 2015 Myles Salholm */
#pragma once

#include "Definitions.h"

#if WINDOWS
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#endif

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
public:
	/* The window class this program is using. */
	WNDCLASSEX WindowsClassEX;

	/* The name of the program. */
	TCHAR* WindowClass;
	/* Title of the program. */
	TCHAR* Title;

	/* The window handle for this program. */
	HWND WindowHandle;

	/* First time setup for windows programs.  Returns true if successful. */
	bool InitWindows(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

	/* The main execution loop for the program. */
	bool ProgramExecutionLoopWindows(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
#endif
};