/* Copyright 2015 Myles Salholm */
#pragma once

#include "Runtime/Public/Definitions.h"

#if DIRECTX
#include "Runtime/Public/Graphics/D3DHandle.h"
#endif

/* 
The context which determines how the rendering engine behaves.
*/
class FRenderContext
{
public:
	/* Constructor. */
	FRenderContext();
	/* Destructor. */
	~FRenderContext();

	/* Initialized the render context. */
	bool Init(class FWindow* Owner);

	/* Shuts down the render context. */
	void Shutdown();

	/* Processes a single frame. */
	bool Frame();

	/* Processes a single frame. */
	bool Render();

	/* The window that owns this render context. */
	class FWindow* WindowOwner;

private:
#if DIRECTX
	/* This handles interaction with DirectX. */
	FD3DHandle* DirectXHandle;
#endif
};