/* Copyright 2015 Myles Salholm */
#include "Runtime/Public/Core/ProgramInstance.h"
#include "Runtime/Public/Core/GameInstance.h"
#include "Runtime/Public/Input/InputHandle.h"
#include "Runtime/Public/Core/Window.h"
#include "Runtime/Public/Graphics/RenderContext.h"

/* The global InputHandle for the program. */
static FInputHandle InputHandle = FInputHandle();

FProgramInstance::FProgramInstance()
{
	ShutdownReason = EShutdownReason::Unknown;
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
		ActiveWindow = nullptr;
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

	// Empty out the message structure
	ZeroMemory(&Message, sizeof(MSG));
#endif
#else
#if WINDOWS
	if (AddGameInstance(AddWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow, DefaultWidth, DefaultHeight)) == nullptr)
	{
		/* Failed to create game. */
		return false;
	}
#endif
#endif

	return true;
}

void FProgramInstance::Loop()
{
	bool renderResult = false;
	unsigned int gameIt = 0;
	unsigned int windowIt = 0;

	// Standard fixed timestep with variable framerate.
	double lastFrameTime = GetTime();
	double lag = 0.0f;
	while (!bShutdown)
	{
		double currentFrameTime = GetTime();
		double elapsedFrameTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;
		lag += elapsedFrameTime;

		ProcessInput();

		while (lag >= MS_PER_UPDATE)
		{
			// Update game world
			for (gameIt = 0; gameIt < GameInstances.Num(); ++gameIt)
			{
				//@TODO GameInstances[GameIt].Update();
			}
			lag -= MS_PER_UPDATE;
		}

		// Process the next frame
		if (bOnlyUpdateActiveWindow)
		{
			if (ActiveWindow)
			{
				if ((!ActiveWindow->Frame()))
				{
					ExecuteShutdown(EShutdownReason::FailedToRender);
				}
			}
			// No active window? No problem.
		}
		else
		{
			for (windowIt = 0; windowIt < Windows.Num(); ++windowIt)
			{
				if (!Windows[windowIt].Frame())
				{
					ExecuteShutdown(EShutdownReason::FailedToRender);
				}
			}
		}
	}
}

FGameInstance* FProgramInstance::AddGameInstance(FWindow* InWindow)
{
	if (!InWindow)
	{
		// No window, let's make one then!
		return nullptr;
	}
	FGameInstance* newGameInstance = new FGameInstance();
	if (newGameInstance)
	{
		if (newGameInstance->Init(InWindow))
		{
			GameInstances.Add(*newGameInstance);
			if (!Windows.Contains(*InWindow))
			{
				Windows.Add(*InWindow);
				ActiveWindow = InWindow;
			}
			return newGameInstance;
		}
		else
		{
			// Failed to initialize game instance
			delete newGameInstance;
		}
	}
	// Somehow failed to make the game instance
	return nullptr;
}

void FProgramInstance::ExecuteShutdown(EShutdownReason::Type Reason)
{
	bShutdown = true;
	ShutdownReason = Reason;
}

void FProgramInstance::Cleanup()
{
	GameInstances.Empty();
	Windows.Empty();
	if (ActiveWindow)
	{
		delete ActiveWindow;
		ActiveWindow = nullptr;
	}

	//@TODO InputHandle.Cleanup();
}

void FProgramInstance::ProcessInput()
{

#if WINDOWS

	if (PeekMessage(&Message, ActiveWindow->WindowHandle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	if (Message.message == WM_QUIT)
	{
		ExecuteShutdown(EShutdownReason::Quit);
	}

	//@TODO Move this to a more permanent place.  This is prototype code, meant to be removed.
	if (GetInputHandle()->IsKeyDown(VK_ESCAPE))
	{
		ExecuteShutdown(EShutdownReason::Quit);
	}
	if (GetInputHandle()->IsKeyDown(VK_SHIFT))
	{
		ActiveWindow->RenderContext->bRenderWithTexture = false;
	}
	if (GetInputHandle()->IsKeyDown(VK_CONTROL))
	{
		ActiveWindow->RenderContext->bRenderWithTexture = true;
	}
#endif
}

FInputHandle* FProgramInstance::GetInputHandle()
{
	return &InputHandle;
}

double FProgramInstance::GetTime()
{
#if WINDOWS
	return GetCurrentTime();
#else
	// Unknown platform.
	return 0.0f;
#endif
}

#if WINDOWS
LRESULT CALLBACK WndProc(HWND InWindowHandle, UINT InMessage, WPARAM wParam, LPARAM lParam)
{
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

FWindow* FProgramInstance::AddWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow, int Width, int Height)
{
	FWindow* newWindow = new FWindow();
	if (newWindow)
	{
		if (newWindow->InitWindows(hInstance, hPrevInstance, lpCmdLine, nCmdShow, Width, Height, WndProc))
		{
			Windows.Add(*newWindow);
			ActiveWindow = newWindow;
			return newWindow;
		}
		else
		{
			// Window initialization failed
			delete newWindow;
		}
	}
	return nullptr;
}
#endif