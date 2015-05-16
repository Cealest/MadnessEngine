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
class FColorShader
{
private:
#if DIRECTX
	struct SMatrixBuffer
	{
		D3DXMATRIX World;
		D3DXMATRIX View;
		D3DXMATRIX Projection;
	};
#endif

public:
	FColorShader();
	FColorShader(const FColorShader &Other);
	~FColorShader();

#if DIRECTX
	
	bool Initialize(ID3D11Device* InDevice, HWND InWindowHandle);
	void Shutdown();
	bool Render(ID3D11DeviceContext* InDeviceContext, int InIndexCount, D3DXMATRIX InWorldMatrix, D3DXMATRIX InViewMatrix, D3DXMATRIX InProjectionMatrix);

private:
	bool InitializeShader(ID3D11Device* InDevice, HWND InWindowHandle, WCHAR* InVertexShaderFile, WCHAR* InPixelShaderFile);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* InErrorMessage, HWND InWindowHandle, WCHAR* InShaderFilename);

	bool SetShaderParameters(ID3D11DeviceContext* InDeviceContext, D3DXMATRIX InWorldMatrix, D3DXMATRIX InViewMatrix, D3DXMATRIX InProjectionMatrix);
	void RenderShader(ID3D11DeviceContext* InDeviceContext, int InIndexCount);

	ID3D11VertexShader* VertexShader;
	ID3D11PixelShader* PixelShader;
	ID3D11InputLayout* Layout;
	ID3D11Buffer* MatrixBuffer;

#endif
};