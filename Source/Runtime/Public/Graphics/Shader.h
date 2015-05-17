/* Copyright 2015 Myles Salholm */
#pragma once

#include "Runtime/Public/Definitions.h"

#include <fstream>

#if DIRECTX
#include <d3d11.h>
#include <D3DX10math.h>
#include <d3dx11async.h>
#endif

// A simple color shader
class FShader
{
protected:
#if DIRECTX
	struct SMatrixBuffer
	{
		D3DXMATRIX World;
		D3DXMATRIX View;
		D3DXMATRIX Projection;
	};
#endif

public:
	FShader();
	FShader(const FShader &Other);
	~FShader();

#if DIRECTX

	virtual bool Initialize(ID3D11Device* InDevice, HWND InWindowHandle);
	virtual void Shutdown();
	virtual bool Render(ID3D11DeviceContext* InDeviceContext, int InIndexCount, D3DXMATRIX InWorldMatrix, D3DXMATRIX InViewMatrix, D3DXMATRIX InProjectionMatrix);

protected:
	virtual bool InitializeShader(ID3D11Device* InDevice, HWND InWindowHandle, WCHAR* InVertexShaderFile, WCHAR* InPixelShaderFile);
	virtual void ShutdownShader();
	virtual void OutputShaderErrorMessage(ID3D10Blob* InErrorMessage, HWND InWindowHandle, WCHAR* InShaderFilename);

	virtual bool SetShaderParameters(ID3D11DeviceContext* InDeviceContext, D3DXMATRIX InWorldMatrix, D3DXMATRIX InViewMatrix, D3DXMATRIX InProjectionMatrix);
	virtual void RenderShader(ID3D11DeviceContext* InDeviceContext, int InIndexCount);

	// File location for the vertex shader.
	WCHAR* VertexShaderFile;
	// File location for the pixel shader.
	WCHAR* PixelShaderFile;

	// Name of the function to run on vertexes
	LPCSTR VertexFunctionName;
	// Name of the function to run on pixels
	LPCSTR PixelFunctionName;

	ID3D11VertexShader* VertexShader;
	ID3D11PixelShader* PixelShader;
	ID3D11InputLayout* Layout;
	ID3D11Buffer* MatrixBuffer;

#endif
};