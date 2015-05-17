/* Copyright 2015 Myles Salholm */
#pragma once

#include "Runtime/Public/Definitions.h"

#include <wchar.h>

#if DIRECTX
#include <d3d11.h>
#include <D3DX10math.h>
#endif

/* Holds data to render a model. */
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
	};

	struct SColorVertex : public SVertex
	{
		D3DXVECTOR4 Color;
	};

	struct STextureVertex : public SVertex
	{
		D3DXVECTOR2 Texture;
	};

	//@TODO Isolate to DirectX Children
	bool Initialize(ID3D11Device* InDevice, WCHAR* InTextureFilename = L"");
	void Shutdown();
	void Render(ID3D11DeviceContext* InDeviceContext);

	int GetIndexCount();

	class FTexture* GetTexture() const;

	const bool UsesTexture() const;

private:
	bool InitializeBuffers(ID3D11Device* InDevice);
	bool InitializeBuffersColored(ID3D11Device* InDevice);
	bool InitializeBuffersTextured(ID3D11Device* InDevice);

	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* InDeviceContext);

	bool LoadTexture(ID3D11Device* InDevice, WCHAR* InFile);
	void ReleaseTexture();

private:
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;
	int VertexCount;
	int IndexCount;

	class FTexture* Texture;
#endif
};