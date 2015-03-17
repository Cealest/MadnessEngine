/* Copyright 2015 Myles Salholm */
#pragma once

#include "Runtime/Public/Definitions.h"

#if DIRECTX
/* This is the first place we actually include the DirectX libraries. */
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <D3DX10math.h>

/*
Handles interactions with DirectX.
*/
class FD3DHandle
{
public:
	/* Constructor. */
	FD3DHandle();
	/* Destructor. */
	~FD3DHandle();
};
#endif