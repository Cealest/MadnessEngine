/* Copyright 2015 Myles Salholm */

#pragma once

#include "Runtime/Public/Definitions.h"
#if DIRECTX
#include "Runtime/Public/Graphics/D3DHandle.h"
#endif

class FRenderContext
{
public:
	/* Constructor. */
	FRenderContext();
	/* Destructor. */
	~FRenderContext();

	/* Processes a single frame. */
	virtual bool Frame();

private:
#if DIRECTX
	/* This handles interaction with DirectX. */
	FD3DHandle* DirectXHandle;
#endif
};