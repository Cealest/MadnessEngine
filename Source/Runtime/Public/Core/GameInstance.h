/* Copyright 2015 Myles Salholm */
#pragma once

#include "Runtime/Public/Definitions.h"
#include "Runtime/Public/Core/Window.h"

class FController;

/*
GameInstance is a single game in the program.
There is normally only a single game in the program.
*/
class FGameInstance
{
public:
	/* Constructor. */
	FGameInstance();
	/* Destructor. */
	~FGameInstance();

	/* Initializes the game instance and gives it a window. */
	virtual bool Init(FWindow* Window);

	virtual void Update();

	/* Returns the window if one exists.  If this return nullptr, the instance probably hasn't been initialized. */
	virtual FWindow* GetWindow() const;
	virtual FController* GetController() const;

protected:
	/* The window for this game instance. */
	FWindow* Window;

	FController* Controller;
};