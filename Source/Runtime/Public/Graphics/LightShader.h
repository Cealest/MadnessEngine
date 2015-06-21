/* Copyright 2015 Myles Salholm */
#pragma once

#include "Runtime/Public/Definitions.h"

#include "Runtime/Public/Graphics/TextureShader.h"
#include "Runtime/Public/Graphics/Light.h"

// A simple color shader
class FLightShader : public FTextureShader
{
public:
	FLightShader();
	FLightShader(const FLightShader &Other);
	~FLightShader();

	virtual EShader::Type GetShaderType() const override { return EShader::DiffuseLight; }

#if DIRECTX
protected:
	struct SLightBuffer
	{
		D3DXVECTOR4 DiffuseColor;
		D3DXVECTOR3 LightDirection;
		// Extra padding so structure is a multiple of 16 for CreateBuffer function requirements.
		float Padding;
	};
private:
	// Hide away these overload
	bool Render(ID3D11DeviceContext* InDeviceContext, int InIndexCount, D3DXMATRIX InWorldMatrix, D3DXMATRIX InViewMatrix, D3DXMATRIX InProjectionMatrix) override { return false; }
	virtual bool SetShaderParameters(ID3D11DeviceContext* InDeviceContext, D3DXMATRIX InWorldMatrix, D3DXMATRIX InViewMatrix, D3DXMATRIX InProjectionMatrix) override { return false; }

public:
	bool Render(ID3D11DeviceContext* InDeviceContext, int InIndexCount, D3DXMATRIX InWorldMatrix, D3DXMATRIX InViewMatrix, D3DXMATRIX InProjectionMatrix, FLight* InLight);

protected:
	virtual bool InitializeShader(ID3D11Device* InDevice, HWND InWindowHandle, WCHAR* InVertexShaderFile, WCHAR* InPixelShaderFile) override;
	virtual bool SetShaderParameters(ID3D11DeviceContext* InDeviceContext, D3DXMATRIX InWorldMatrix, D3DXMATRIX InViewMatrix, D3DXMATRIX InProjectionMatrix, FLight* InLight);
	virtual void ShutdownShader() override;

	virtual void RenderShader(ID3D11DeviceContext* InDeviceContext, int InIndexCount) override;

	ID3D11Buffer* LightBuffer;
#endif
};