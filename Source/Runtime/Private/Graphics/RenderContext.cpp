/* Copyright 2015 Myles Salholm */
#include "Runtime/Public/Graphics/RenderContext.h"
#include "Runtime/Public/Core/Window.h"

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
		//DirectXHandle->Shutdown();
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
	//@TODO DirectXHandle->Init
#endif

	return true;
}

void FRenderContext::Shutdown()
{
#if DIRECTX
	if (DirectXHandle)
	{
		//DirectXHandle->Shutdown();
		delete DirectXHandle;
		DirectXHandle = nullptr;
	}
#endif
}

bool FRenderContext::Frame()
{
#if DIRECTX
	if (DirectXHandle)
	{
		//DirectXHandle->BeginScene

		//DirectXHandle->EndScene

		return true;
	}
#endif
	return true;
}