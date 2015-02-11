/* Copyright 2015 Myles Salholm */
#include "Runtime/Public/Core/ProgramInstance.h"

FProgramInstance::FProgramInstance()
{
	ShutdownReason = 0;
	Window = nullptr;
	GameInstance = nullptr;
}

FProgramInstance::~FProgramInstance()
{
	if (Window)
	{
		delete Window;
	}
	if (GameInstance)
	{
		delete GameInstance;
	}
}

#if WINDOWS
bool FProgramInstance::ProgramExecutionLoopWindows(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (Window)
	{
		return Window->WindowExecutionLoopWindows(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	}
	// No window
	ShutdownReason = 1;
	return false;
}

FWindow* FProgramInstance::AddWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (!Window)
	{
		Window = new FWindow();
		if (Window)
		{
			Window->InitWindows(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
			return Window;
		}
	}
	return nullptr;
}
#endif