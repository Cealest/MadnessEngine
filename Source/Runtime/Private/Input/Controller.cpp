/* Copyright 2015 Myles Salholm */
#include "Runtime/Public/Input/Controller.h"
#include "Runtime/Public/Core/ProgramInstance.h"
#include "Runtime/Public/Input/InputHandle.h"
#include "Runtime/Public/Graphics/Camera.h"
#if WINDOWS
#include <windows.h>
#endif

#if !defined(DELETE_ME)
#include "Runtime/Public/Tools/ModelImport.h"
#endif

void FController::FMoveForwardObserver::OnNotify()
{
	FCamera* camera = Controller->Camera;
	if (camera)
	{
		camera->MoveLocalSpace(0, 0, 0.1f);
	}
}

void FController::FMoveLeftObserver::OnNotify()
{
	FCamera* camera = Controller->Camera;
	if (camera)
	{
		camera->MoveLocalSpace(-0.1f, 0, 0);
	}
}

void FController::FMoveRightObserver::OnNotify()
{
	FCamera* camera = Controller->Camera;
	if (camera)
	{
		camera->MoveLocalSpace(0.1f, 0, 0);
	}
}

void FController::FMoveBackObserver::OnNotify()
{
	FCamera* camera = Controller->Camera;
	if (camera)
	{
		camera->MoveLocalSpace(0, 0, -0.1f);
	}
}

void FController::FMouseMoveObserver::OnNotify()
{
	FCamera* camera = Controller->Camera;
	if (camera)
	{
		FInputHandle* inputHandle = GProgramInstance.GetInputHandle();
		if (inputHandle && inputHandle->GetMouse().IsLeftMouseDown())
		{
			const float rotationScale = 0.1f;
			Vector2D pos = inputHandle->GetMouse().GetPosition();
			Vector2D oldPos = inputHandle->GetMouse().GetLastPosition();
			Vector2D deltaPos = pos - oldPos;
			camera->Rotate(-deltaPos.Y * rotationScale, -deltaPos.X * rotationScale, 0);
		}
	}
}

#if !defined(DELETE_ME)
void FController::FImportObserver::OnNotify()
{
	ModelImport importer;
	importer.Import("C:\\MadnessEngine\\MadnessEngine\\Content\\Meshes\\sphere.fbx");
}
#endif

FController::FController()
{
	Camera = nullptr;
	MoveForwardObserver.Controller = this;
	MoveLeftObserver.Controller = this;
	MoveRightObserver.Controller = this;
	MoveBackObserver.Controller = this;
	MouseMoveObserver.Controller = this;
}

FController::~FController()
{

}

bool FController::Initialize()
{
	FInputHandle* inputHandle = GProgramInstance.GetInputHandle();
	if (inputHandle == nullptr)
		return false;
	inputHandle->SubscribeToKeyPress(MoveForwardObserver, EKey::W);
	inputHandle->SubscribeToKeyPress(MoveLeftObserver, EKey::A);
	inputHandle->SubscribeToKeyPress(MoveRightObserver, EKey::D);
	inputHandle->SubscribeToKeyPress(MoveBackObserver, EKey::S);
	inputHandle->SubscribeToMouseMove(MouseMoveObserver);
#if !defined(DELETE_ME)
	inputHandle->SubscribeToKeyPress(ImportObserver, EKey::I);
#endif

	return true;
}

void FController::SetCamera(FCamera* InCamera)
{
	Camera = InCamera;
}

void FController::Update()
{
	FInputHandle* inputHandle = GProgramInstance.GetInputHandle();
	if (Camera && inputHandle)
	{
		const float movementSpeed = 0.1f;
		const float rotationSpeed = 0.5f;
		float xMov = 0;
		float zMov = 0;
		float yRot = 0;
		if (inputHandle->IsKeyDown(EKey::W))
		{
			zMov += movementSpeed;
		}
		if (inputHandle->IsKeyDown(EKey::A))
		{
			xMov -= movementSpeed;
		}
		if (inputHandle->IsKeyDown(EKey::S))
		{
			zMov -= movementSpeed;
		}
		if (inputHandle->IsKeyDown(EKey::D))
		{
			xMov += movementSpeed;
		}
		if (inputHandle->IsKeyDown(EKey::Q))
		{
			yRot -= rotationSpeed;
		}
		if (inputHandle->IsKeyDown(EKey::E))
		{
			yRot += rotationSpeed;
		}
		Camera->MoveLocalSpace(xMov, 0, zMov);
		Camera->Rotate(0, yRot, 0);
	}
}