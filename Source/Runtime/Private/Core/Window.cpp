/* Copyright 2015 Myles Salholm */
#include "Runtime/Public/Core/Window.h"

FWindow::FWindow()
{
#if WINDOWS
	WindowClass = _T(SHORT_TITLE);
	Title = _T(TITLE);
#endif
}

FWindow::~FWindow()
{

}

#if WINDOWS
LRESULT CALLBACK WndProc(HWND InWindowHandle, UINT InMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT PaintStruct;
	HDC HandleDeviceContext;
	TCHAR Greeting[] = _T("Hello, World!");

	switch (InMessage)
	{
	case WM_PAINT:
		HandleDeviceContext = BeginPaint(InWindowHandle, &PaintStruct);

		// Here your application is laid out.
		// For this introduction, we just print out "Hello, World!"
		// in the top left corner.
		TextOut(HandleDeviceContext,
			5, 5,
			Greeting, (int)_tcslen(Greeting));
		// End application-specific layout section.

		EndPaint(InWindowHandle, &PaintStruct);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(InWindowHandle, InMessage, wParam, lParam);
		break;
	}

	return 0;
}

bool FWindow::InitWindows(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	// Setup the window class.
	WindowsClassEX.cbSize = sizeof(WNDCLASSEX);
	WindowsClassEX.style = CS_HREDRAW | CS_VREDRAW;
	WindowsClassEX.lpfnWndProc = WndProc;
	WindowsClassEX.cbClsExtra = 0;
	WindowsClassEX.cbWndExtra = 0;
	WindowsClassEX.hInstance = hInstance;
	WindowsClassEX.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	WindowsClassEX.hCursor = LoadCursor(NULL, IDC_ARROW);
	WindowsClassEX.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WindowsClassEX.lpszMenuName = NULL;
	WindowsClassEX.lpszClassName = WindowClass;
	WindowsClassEX.hIconSm = LoadIcon(WindowsClassEX.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	if (!RegisterClassEx(&WindowsClassEX))
	{
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"), Title, NULL);
		return false;
	}

	// Now setup the handle for the window.
	WindowHandle = CreateWindow(
		WindowClass,
		Title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		500, 100,
		NULL,
		NULL,
		hInstance,
		NULL
		);
	if (!WindowHandle)
	{
		MessageBox(NULL, _T("Call to CreateWindow failed in FProgramInstance::InitWindows!"), Title, NULL);
		return false;
	}

	ShowWindow(WindowHandle, nCmdShow);
	UpdateWindow(WindowHandle);

	return true;
}

int FWindow::WindowExecutionLoopWindows(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	MSG Message;
	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return (int)Message.wParam;
}
#endif