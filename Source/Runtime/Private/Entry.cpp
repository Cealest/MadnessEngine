/* Copyright 2015 Myles Salholm */
#include "Runtime/Public/Core/ProgramInstance.h"

/* The true entry into this madness! */
int MadnessMain()
{
	int StopExecutionReason = EShutdownReason::None;

	// Initialize the program.
	if (!GProgramInstance.Init())
	{
		StopExecutionReason = EShutdownReason::FailedToInit;
	}
	else
	{
		// Begin the program loop.
		GProgramInstance.Loop();
		StopExecutionReason = GProgramInstance.ShutdownReason;

		// Cleanup the program on the way out.
		GProgramInstance.Cleanup();
	}

	return StopExecutionReason;
}

#if WINDOWS
/*
The actual entry point for Windows applications.
*/
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	// Copy out these variables so we can reference them easily.
	GProgramInstance.hInstance = hInstance;
	GProgramInstance.hPrevInstance = hPrevInstance;
	GProgramInstance.lpCmdLine = lpCmdLine;
	GProgramInstance.nCmdShow = nCmdShow;

	return MadnessMain();
}
#else
int main(int argc, char** argv)
{
	return MadnessMain();
}
#endif