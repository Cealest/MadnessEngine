/* Copyright 2015 Myles Salholm */
#pragma once

#include "Runtime/Public/Definitions.h"

#include "Runtime/Public/Graphics/Shader.h"

// A simple color shader
class FTextureShader : public FShader
{
public:
	FTextureShader();
	FTextureShader(const FTextureShader &Other);
	~FTextureShader();

	virtual EShader::Type GetShaderType() const override { return EShader::Texture; }

#if DIRECTX
private:
	virtual bool Initialize(ID3D11Device* InDevice, HWND InWindowHandle) override { return false; }

public:
	void SetTexture(class FTexture* InTexture);
	virtual bool Initialize(ID3D11Device* InDevice, HWND InWindowHandle, class FTexture* InTexture);

protected:
	virtual bool InitializeShader(ID3D11Device* InDevice, HWND InWindowHandle, WCHAR* InVertexShaderFile, WCHAR* InPixelShaderFile) override;
	virtual void ShutdownShader() override;

	virtual bool SetShaderParameters(ID3D11DeviceContext* InDeviceContext, D3DXMATRIX InWorldMatrix, D3DXMATRIX InViewMatrix, D3DXMATRIX InProjectionMatrix) override;
	virtual void RenderShader(ID3D11DeviceContext* InDeviceContext, int InIndexCount) override;

	// The sampler state
	ID3D11SamplerState* SampleState;

	// The texture to render
	class FTexture* Texture;
#endif
};