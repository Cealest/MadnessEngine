/* Copyright 2015 Myles Salholm */
#include "Runtime/Public/Core/ProgramInstance.h"

FProgramInstance& GProgramInstance = FProgramInstance();

/* The true entry into this madness! */
int MadnessMain()
{
	// Initialize the program.
	if (!GProgramInstance.Init())
	{
		GProgramInstance.ShutdownReason = EShutdownReason::FailedToInit;
	}
	else
	{
		// Begin the program loop.
		GProgramInstance.Loop();

		// Cleanup the program on the way out.
		GProgramInstance.Cleanup();
	}

	return GProgramInstance.ShutdownReason;
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