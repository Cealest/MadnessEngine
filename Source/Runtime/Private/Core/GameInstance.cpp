/* Copyright 2015 Myles Salholm */
#include "Runtime/Public/Core/GameInstance.h"

FGameInstance::FGameInstance()
{
	Window = nullptr;
}

FGameInstance::~FGameInstance()
{
	if (Window)
	{
		delete Window;
	}
}

bool FGameInstance::Init(FWindow* InWindow)
{
	if (Window)
	{
		// Already initialized.
		return false;
	}

	if (InWindow)
	{
		Window = InWindow;
		return true;
	}
	return false;
}

FWindow* FGameInstance::GetWindow() const
{
	return Window;
}