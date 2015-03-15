/* Copyright 2015 Myles Salholm */

#pragma once

#include "Runtime/Public/Definitions.h"
#if DIRECTX
//#include "d3d11.h"
//#include "d3dx11.h"
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
};