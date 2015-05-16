/* Copyright 2015 Myles Salholm */
#pragma once

#include "Runtime/Public/Definitions.h"

#if DIRECTX
#include <d3d11.h>
#include <D3DX10math.h>
#endif

/* Holds data to render a model. */
//@TODO Make abstract virtual to house API specifics.
class FModel
{
public:
	FModel();
	FModel(const FModel&);
	~FModel();

#if DIRECTX
public:
	//@TODO Make this non-DirectX specific
	struct SVertex
	{
		D3DXVECTOR3 Position;
		D3DXVECTOR4 Color;
	};

	//@TODO Isolate to DirectX Children
	bool Initialize(ID3D11Device* InDevice);
	void Shutdown();
	void Render(ID3D11DeviceContext* InDeviceContext);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device* InDevice);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* InDeviceContext);

private:
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;
	int VertexCount;
	int IndexCount;
#endif
};