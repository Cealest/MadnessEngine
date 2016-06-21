/* Copyright 2015 Myles Salholm */
#pragma once

#include "Runtime/Public/Definitions.h"

#include <wchar.h>

#if DIRECTX
#include <d3d11.h>
#include <D3DX10math.h>
#include <fstream>
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
	struct D3DVertex
	{
		D3DXVECTOR3 Position;
	};

	struct D3DColorVertex : public D3DVertex
	{
		D3DXVECTOR4 Color;
	};

	struct D3DTextureVertex : public D3DVertex
	{
		D3DXVECTOR2 Texture;
	};

	struct D3DLitTextureVertex : public D3DTextureVertex
	{
		D3DXVECTOR3 Normal;
	};

	struct SVertex
	{
		float LocX, LocY, LocZ;
		float TexU, TexV;
		float NormalX, NormalY, NormalZ;
	};

	struct SMesh
	{
		SMesh()
		{
			Vertices = nullptr;
		}
		SMesh(int Count)
		{
			VertexCount = Count;
			Vertices = new SVertex[VertexCount];
		}
		~SMesh()
		{
			if (Vertices)
			{
				delete[] Vertices;
				Vertices = nullptr;
			}
		}

		SVertex* Vertices;
		int VertexCount;
	};

	//@TODO Isolate to DirectX Children
	bool Initialize(ID3D11Device* InDevice, const char* InModelFilename, WCHAR* InTextureFilename = L"");
	void Shutdown();
	void Render(ID3D11DeviceContext* InDeviceContext);

	int GetIndexCount();

	class FTexture* GetTexture() const;

	const bool UsesTexture() const;

private:
	bool InitializeBuffers(ID3D11Device* InDevice, EShader::Type InShaderType);
	bool InitializeBuffersColored(ID3D11Device* InDevice);
	bool InitializeBuffersTextured(ID3D11Device* InDevice);
	bool InitializeBuffersLitTextured(ID3D11Device* InDevice);

	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* InDeviceContext);

	bool LoadTexture(ID3D11Device* InDevice, WCHAR* InFile);
	void ReleaseTexture();

	bool LoadModel(const char* InFile);
	void ReleaseModel();

private:
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;
	int VertexCount;
	int IndexCount;

	class FTexture* Texture;
	SMesh* Mesh;
	EShader::Type ActiveShaderType;
#endif
};