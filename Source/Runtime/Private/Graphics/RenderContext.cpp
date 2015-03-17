/* Copyright 2015 Myles Salholm */
#include "Runtime/Public/Graphics/RenderContext.h"

FRenderContext::FRenderContext()
{
	DirectXHandle = nullptr;
}

FRenderContext::~FRenderContext()
{
	if (DirectXHandle)
	{
		delete DirectXHandle;
		DirectXHandle = nullptr;
	}
}

bool FRenderContext::Frame()
{
	return true;
}