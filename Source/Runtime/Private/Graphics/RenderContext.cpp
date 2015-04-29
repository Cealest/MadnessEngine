/* Copyright 2015 Myles Salholm */
#include "Runtime/Public/Graphics/RenderContext.h"
#include "Runtime/Public/Core/Window.h"
#include "Runtime/Public/Core/ProgramInstance.h"

FRenderContext::FRenderContext()
{
	WindowOwner = nullptr;

#if DIRECTX
	DirectXHandle = nullptr;
#endif
}

FRenderContext::~FRenderContext()
{
#if DIRECTX
	if (DirectXHandle)
	{
		DirectXHandle->Shutdown();
		delete DirectXHandle;
		DirectXHandle = nullptr;
	}
#endif

	WindowOwner = nullptr;
}

bool FRenderContext::Init(FWindow* Owner)
{
	WindowOwner = Owner;
	if (!WindowOwner)
	{
		// No window owner, we don't know where to render to.
		return false;
	}

#if DIRECTX
	DirectXHandle = new FD3DHandle();
	if (!DirectXHandle)
	{
		// No DirectX interface and we're using DirectX.
		return false;
	}
	if (!DirectXHandle->Initialize(GProgramInstance.DefaultWidth, GProgramInstance.DefaultHeight, GVerticalSync, Owner->WindowHandle, GFarClip, GNearClip))
	{
		// Failed to initialize DirectX
		return false;
	}
#endif

	return true;
}

void FRenderContext::Shutdown()
{
#if DIRECTX
	if (DirectXHandle)
	{
		DirectXHandle->Shutdown();
		delete DirectXHandle;
		DirectXHandle = nullptr;
	}
#endif
}

bool FRenderContext::Frame()
{
	//@TODO Re-evaluate whether this function is really necessary.

	bool result = Render();
	if (!result)
	{
		// Failed to render.
		return false;
	}

	return true;
}

bool FRenderContext::Render()
{
#if DIRECTX
	if (DirectXHandle)
	{
		// Clear the buffers.
		DirectXHandle->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

		// Swap the buffers.
		DirectXHandle->EndScene();

		return true;
	}
#endif
	return true;
}