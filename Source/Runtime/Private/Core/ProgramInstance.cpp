/* Copyright 2015 Myles Salholm */
#include "Runtime/Public/Core/ProgramInstance.h"
#include "Runtime/Public/Core/GameInstance.h"

FProgramInstance::FProgramInstance()
{
	ShutdownReason = 0;
	GameInstances.Empty();
	Windows.Empty();
}

FProgramInstance::~FProgramInstance()
{
	GameInstances.Empty();
	Windows.Empty();
}

FGameInstance* FProgramInstance::AddGameInstance(FWindow* InWindow)
{
	if (InWindow)
	{
		// No window, failed!
		return nullptr;
	}
	FGameInstance* NewGameInstance = new FGameInstance();
	if (NewGameInstance)
	{
		if (NewGameInstance->Init(InWindow))
		{
			GameInstances.Add(*NewGameInstance);
			if (!Windows.Contains(*InWindow))
			{
				Windows.Add(*InWindow);
			}
			return NewGameInstance;
		}
		else
		{
			// Failed to initialize game instance
			delete NewGameInstance;
		}
	}
	// Somehow failed to make the game instance
	return nullptr;
}

#if WINDOWS
int FProgramInstance::ProgramExecutionLoopWindows(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	for (unsigned int i = 0; i < Windows.Num(); ++i)
	{
		int returnVal = ShutdownReason;
		if (Windows.Get(i))
		{
			ShutdownReason = Windows.Get(i)->WindowExecutionLoopWindows(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
		}
	}

	return ShutdownReason;
}

FWindow* FProgramInstance::AddWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	FWindow* NewWindow = new FWindow();
	if (NewWindow)
	{
		if (NewWindow->InitWindows(hInstance, hPrevInstance, lpCmdLine, nCmdShow))
		{
			Windows.Add(*NewWindow);
			return NewWindow;
		}
		else
		{
			// Window initialization failed
			delete NewWindow;
		}
	}
	return nullptr;
}
#endif