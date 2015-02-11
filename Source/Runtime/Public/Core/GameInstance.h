/* Copyright 2015 Myles Salholm */
#pragma once

#include "Runtime/Public/Definitions.h"
#include "Runtime/Public/Core/Window.h"

/*
GameInstance is a single game window in the program.
*/
class FGameInstance
{
public:
	/* Constructor. */
	FGameInstance();
	/* Destructor. */
	~FGameInstance();

	FWindow* Window;
};