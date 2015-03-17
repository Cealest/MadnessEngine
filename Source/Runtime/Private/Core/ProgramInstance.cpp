/* Copyright 2015 Myles Salholm */
#include "Runtime/Public/Core/ProgramInstance.h"
#include "Runtime/Public/Core/GameInstance.h"
#include "Runtime/Public/Input/InputHandle.h"

/* The global InputHandle for the program. */
static FInputHandle InputHandle = FInputHandle();

FProgramInstance::FProgramInstance()
{
	ShutdownReason = 0;
	GameInstances.Empty();
	Windows.Empty();
	ActiveWindow = nullptr;
	bShutdown = false;
}

FProgramInstance::~FProgramInstance()
{
	GameInstances.Empty();
	Windows.Empty();
	if (ActiveWindow)
	{
		delete ActiveWindow;
	}
}

bool FProgramInstance::Init()
{
	/* Setup our input handle. */
	InputHandle.Init();

#if WITH_EDITOR
#if WINDOWS
	if (!AddWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow, DefaultWidth, DefaultHeight))
	{
		/* Failed to add window. */
		return false;
	}
#endif
#else
	if (!AddGameInstance())
	{
		/* Failed to create game. */
		return false;
	}
#endif

	return true;
}

void FProgramInstance::Loop()
{
#if WINDOWS
	ProgramExecutionLoopWindows(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
#endif
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
				ActiveWindow = InWindow;
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

void FProgramInstance::ExecuteShutdown(int Reason)
{
	bShutdown = true;
	ShutdownReason = Reason;
}

void FProgramInstance::Cleanup()
{

}

FInputHandle* FProgramInstance::GetInputHandle()
{
	return &InputHandle;
}

#if WINDOWS
LRESULT CALLBACK WndProc(HWND InWindowHandle, UINT InMessage, WPARAM wParam, LPARAM lParam)
{
	TCHAR Greeting[] = _T("Hello, World!");

	switch (InMessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		// Call this window's message handler.
		return GProgramInstance.HandleMessage(InWindowHandle, InMessage, wParam, lParam);
	}

	return 0;
}

LRESULT CALLBACK FProgramInstance::HandleMessage(HWND& InWindowHandle, UINT& InMessage, WPARAM& wParam, LPARAM& lParam)
{
	switch (InMessage)
	{
	case WM_KEYDOWN:
		InputHandle.KeyDown((unsigned int)wParam);
		return 0;
	case WM_KEYUP:
		InputHandle.KeyUp((unsigned int)wParam);
		return 0;
	default:
		return DefWindowProc(InWindowHandle, InMessage, wParam, lParam);
	}
}

int FProgramInstance::ProgramExecutionLoopWindows(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG Message;
	bool Done, Result;
	int GameIt = 0;

	// Empty out the message structure
	ZeroMemory(&Message, sizeof(MSG));

	Done = false;
	Result = false;

	while (!Done)
	{
		// Process Input
		if (!ActiveWindow)
		{
			// No active window
		}
		if (PeekMessage(&Message, ActiveWindow->WindowHandle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		if (Message.message == WM_QUIT)
		{
			Done = true;
		}
		else
		{
			// Update game world
			for (GameIt = 0; GameIt < GameInstances.Num(); ++GameIt)
			{
				//@TODO GameInstances[GameIt].Update();
			}

			// Process the next frame
			Result = ActiveWindow->Frame(&InputHandle);
			if (!Result)
			{
				Done = true;
			}
		}
	}

	return ShutdownReason;
}

FWindow* FProgramInstance::AddWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow, int Width, int Height)
{
	FWindow* NewWindow = new FWindow();
	if (NewWindow)
	{
		if (NewWindow->InitWindows(hInstance, hPrevInstance, lpCmdLine, nCmdShow, Width, Height, WndProc))
		{
			Windows.Add(*NewWindow);
			ActiveWindow = NewWindow;
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