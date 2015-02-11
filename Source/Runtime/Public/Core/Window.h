/* Copyright 2015 Myles Salholm */
#pragma once

#include "Runtime/Public/Definitions.h"

#if WINDOWS
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#endif

class FWindow
{
public:
	/* Constructor. */
	FWindow();
	/* Destructor. */
	~FWindow();

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

	/* First time setup for this window.  Returns true if successful. */
	bool InitWindows(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

	/* The main execution loop for this window. */
	int WindowExecutionLoopWindows(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
#endif
};