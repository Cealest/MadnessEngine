/* Copyright 2015 Myles Salholm */
#pragma once

#include "Runtime/Public/Definitions.h"
#include "Runtime/Public/Core/Window.h"
#include "Runtime/Public/Templates/Array.h"

/*
ProgramInstance represents a single instance of the program.  It acts as
a static global wrapper to abstractly interact with the target platform.
There should only be one ProgramInstance per program.
*/
class FProgramInstance
{
public:
	/* Variable constants. */
	const int DefaultWidth = 1024;
	const int DefaultHeight = 768;
	
	const bool bOnlyUpdateActiveWindow = true;

	/* Why we're shutting down the program. 0 means everything went smoothly. */
	EShutdownReason::Type ShutdownReason;

	/* Constructor. */
	FProgramInstance();
	/* Destructor. */
	~FProgramInstance();

	/* Initializes the program. */
	bool Init();

	/* Runs the main loop for the program. */
	void Loop();

	/* Function which creates a game in the specified window. */
	class FGameInstance* AddGameInstance(FWindow* InWindow);

	/* Assigns ShutdownReason */
	void ExecuteShutdown(EShutdownReason::Type Reason);

	/* Cleans up the program after the program has started exiting. */
	void Cleanup();

	/* Processes all input for the program.
	 * Generally, the ActiveWindow will govern what to do with it.
	 * However, input may change the active window.
	 */
	void ProcessInput();

	/* Retrieves the handle for the program's input. */
	class FInputHandle* GetInputHandle();

	/* Returns the current clock cycles in milliseconds. */
	double GetTime();

#if WINDOWS
	/* Distributes the Windows message to the active window. */
	static LRESULT CALLBACK HandleMessage(HWND& InWindowHandle, UINT& InMessage, WPARAM& wParam, LPARAM& lParam);

	/* Function which creates a window not tied to a game instance. */
	FWindow* AddWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow, int Width, int Height);


	HINSTANCE hInstance;
	HINSTANCE hPrevInstance;
	LPSTR lpCmdLine;
	int nCmdShow;

	MSG Message;
#endif

private:
	/* True if a shutdown was requested. */
	bool bShutdown;

	/* An array of game instances running in the program. */
	TArray<FGameInstance> GameInstances;

	/* An array of windows running in the program. */
	TArray<FWindow> Windows;

	/* Which window is currently active. */
	FWindow* ActiveWindow;
};

#if WINDOWS
/* Windows callback procedure. */
static LRESULT CALLBACK WndProc(HWND InWindowHandle, UINT InMessage, WPARAM wParam, LPARAM lParam);
#endif

/* Global program instance. */
extern FProgramInstance& GProgramInstance;