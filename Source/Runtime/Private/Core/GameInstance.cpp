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