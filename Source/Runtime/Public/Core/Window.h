/* Copyright 2015 Myles Salholm */
#pragma once

#include "Runtime/Public/Definitions.h"

#if WINDOWS
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#endif

/*
A single window in the program.
This could be an actual window on an
operating system or it could just be
a renderable screen space.
*/
class FWindow
{
public:
	/* Constructor. */
	FWindow();
	/* Destructor. */
	~FWindow();

	/* Initializes the window's render context. */
	bool Init();

	/* Handles a single frame of execution. */
	bool Frame();

protected:
	/* The context by which graphics are rendered for this window. */
	class FRenderContext* RenderContext;

#if WINDOWS
public:
	/* The window class this program is using. */
	WNDCLASSEX WindowsClassEX;

	/* The name of the window. */
	TCHAR* WindowClass;
	/* Title of the window. */
	TCHAR* Title;

	/* The window handle for this window. */
	HWND WindowHandle;

	/* First time setup for this window.  Returns true if successful. */
	bool InitWindows(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow, int Width, int Height, const WNDPROC& WndProc);

	/* Handles messages sent from Windows. */
	LRESULT CALLBACK MessageHandler(HWND InWindowHandle, UINT InMessage, WPARAM wParam, LPARAM lParam);
#endif
};