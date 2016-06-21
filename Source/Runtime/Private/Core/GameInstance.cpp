/* Copyright 2015 Myles Salholm */
#include "Runtime/Public/Core/GameInstance.h"
#include "Runtime/Public/Input/Controller.h"
#include "Runtime/Public/Core/Window.h"
#include "Runtime/Public/Graphics/RenderContext.h"

FGameInstance::FGameInstance()
{
	Window = nullptr;
	Controller = nullptr;
}

FGameInstance::~FGameInstance()
{
	if (Controller)
	{
		delete Controller;
	}
}

bool FGameInstance::Init(FWindow* InWindow)
{
	if (Window)
	{
		// Already initialized.
		return false;
	}
	Window = InWindow;

	if (Controller)
	{
		// Already initialized.
		return false;
	}
	Controller = new FController();
	Controller->Initialize();
	Controller->SetCamera(Window->RenderContext->GetCamera());

	return true;
}

FController* FGameInstance::GetController() const
{
	return Controller;
}

FWindow* FGameInstance::GetWindow() const
{
	return Window;
}

void FGameInstance::Update()
{
	if (Controller)
	{
		Controller->Update();
	}
}