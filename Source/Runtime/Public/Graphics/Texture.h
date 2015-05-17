/* Copyright 2015 Myles Salholm */
#pragma once

#include "Runtime/Public/Definitions.h"

#if DIRECTX
#include <d3d11.h>
#include <D3DX11tex.h>
#endif

class FTexture
{
public:
	FTexture();
	FTexture(const FTexture& Other);
	~FTexture();

#if DIRECTX
	bool Initialize(ID3D11Device* InDevice, WCHAR* InFile);
	void Shutdown();

	ID3D11ShaderResourceView* Texture;

#endif
};