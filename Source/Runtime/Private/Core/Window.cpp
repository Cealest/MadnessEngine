/* Copyright 2015 Myles Salholm */
#include "Runtime/Public/Core/Window.h"
#include "Runtime/Public/Graphics/RenderContext.h"
#include "Runtime/Public/Input/InputHandle.h"

FWindow::FWindow()
{
	RenderContext = nullptr;

#if WINDOWS
	WindowClass = _T(SHORT_TITLE);
	Title = _T(TITLE);
#endif
}

FWindow::~FWindow()
{
	if (RenderContext)
	{
		RenderContext->Shutdown();
		delete RenderContext;
	}
}

bool FWindow::Init()
{
	RenderContext = new FRenderContext();
	if (!RenderContext)
	{
		// Failed to create render context.
		return false;
	}
	if (!RenderContext->Init(this))
	{
		// Failed to initialize render context.
		return false;
	}
	return true;
}

bool FWindow::Frame()
{
	if (RenderContext)
	{
		if (!RenderContext->Frame())
		{
			return false;
		}
	}

	return true;
}

#if WINDOWS
bool FWindow::InitWindows(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow,
	int Width,
	int Height,
	const WNDPROC& WndProc)
{
	int WindowX, WindowY;

	// Setup the window class.
	WindowsClassEX.cbSize = sizeof(WNDCLASSEX);
	WindowsClassEX.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	WindowsClassEX.lpfnWndProc = WndProc;
	WindowsClassEX.cbClsExtra = 0;
	WindowsClassEX.cbWndExtra = 0;
	WindowsClassEX.hInstance = hInstance;
	WindowsClassEX.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	WindowsClassEX.hIconSm = LoadIcon(WindowsClassEX.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	WindowsClassEX.hCursor = LoadCursor(NULL, IDC_ARROW);
	WindowsClassEX.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WindowsClassEX.lpszMenuName = NULL;
	WindowsClassEX.lpszClassName = WindowClass;
	if (!RegisterClassEx(&WindowsClassEX))
	{
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"), Title, NULL);
		return false;
	}

	// Setup full screen settings
	if (FULLSCREEN)
	{
		Width = GetSystemMetrics(SM_CXSCREEN);
		Height = GetSystemMetrics(SM_CYSCREEN);

		WindowX = 0;
		WindowY = 0;
	}
	else
	{
		WindowX = (GetSystemMetrics(SM_CXSCREEN) - Width) / 2;
		WindowY = (GetSystemMetrics(SM_CYSCREEN) - Height) / 2;
	}

	// Now setup the handle for the window.
	WindowHandle = CreateWindowEx(WS_EX_APPWINDOW,
		WindowClass,
		Title,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		WindowX, WindowY,
		Width, Height,
		NULL,
		NULL,
		hInstance,
		NULL
		);
	if (!WindowHandle)
	{
		MessageBox(NULL, _T("Call to CreateWindowEx failed in FProgramInstance::InitWindows!"), Title, NULL);
		return false;
	}

	ShowWindow(WindowHandle, nCmdShow);
	SetForegroundWindow(WindowHandle);
	SetFocus(WindowHandle);
	ShowCursor(true);

	// Return the generic initialization's result.
	return Init();
}
#endif