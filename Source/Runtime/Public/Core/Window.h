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

	/* Handles a single frame of execution. */
	bool Frame(class FInputHandle* InputHandle);

protected:
	/* The context by which graphics are rendered for this window. */
	class FRenderContext* RenderContext;

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
	bool InitWindows(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow, int Width, int Height, const WNDPROC& WndProc);

	/* Handles messages sent from Windows. */
	LRESULT CALLBACK MessageHandler(HWND InWindowHandle, UINT InMessage, WPARAM wParam, LPARAM lParam);
#endif
};