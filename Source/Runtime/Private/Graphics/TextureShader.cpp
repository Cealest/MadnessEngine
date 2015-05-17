/* Copyright 2015 Myles Salholm */
#include "Runtime/Public/Graphics/TextureShader.h"
#include "Runtime/Public/Graphics/Texture.h"

FTextureShader::FTextureShader() : FShader()
{
#if DIRECTX
	SampleState = nullptr;
	Texture = nullptr;
	VertexShaderFile = L"C:/MadnessEngine/MadnessEngine/Source/Shaders/Texture.vs";
	PixelShaderFile = L"C:/MadnessEngine/MadnessEngine/Source/Shaders/Texture.ps";
	VertexFunctionName = "TextureVertexShader";
	PixelFunctionName = "TexturePixelShader";
#endif
}

FTextureShader::FTextureShader(const FTextureShader &Other) : FShader(Other)
{

}

FTextureShader::~FTextureShader()
{
#if DIRECTX
	// This isn't our managed texture, just don't keep the reference
	Texture = nullptr;
#endif
}

#if DIRECTX

bool FTextureShader::Initialize(ID3D11Device* InDevice, HWND InWindowHandle, class FTexture* InTexture)
{
	if (!InTexture || !InTexture->Texture)
	{
		return false;
	}
	Texture = InTexture;

	return FShader::Initialize(InDevice, InWindowHandle);
}

bool FTextureShader::InitializeShader(ID3D11Device* InDevice, HWND InWindowHandle, WCHAR* InVertexShaderFile, WCHAR* InPixelShaderFile)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	errorMessage = nullptr;
	vertexShaderBuffer = nullptr;
	pixelShaderBuffer = nullptr;

	// Compile the vertex shader code
	result = D3DX11CompileFromFile(InVertexShaderFile, nullptr, nullptr, VertexFunctionName, "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr, &vertexShaderBuffer, &errorMessage, nullptr);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, InWindowHandle, InVertexShaderFile);
		}
		else
		{
			MessageBox(InWindowHandle, InVertexShaderFile, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Compile the pixel shader code
	result = D3DX11CompileFromFile(InPixelShaderFile, nullptr, nullptr, PixelFunctionName, "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr, &pixelShaderBuffer, &errorMessage, nullptr);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, InWindowHandle, InPixelShaderFile);
		}
		else
		{
			MessageBox(InWindowHandle, InPixelShaderFile, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Create the vertex shader from the buffer.
	result = InDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, &VertexShader);
	if (FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = InDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, &PixelShader);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the layout of the data that goes into the shader.
	// Needs to match the SVertex type in FModel and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = InDevice->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &Layout);
	if (FAILED(result))
	{
		return false;
	}

	// Release the buffers that we don't need anymore.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(SMatrixBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer.
	result = InDevice->CreateBuffer(&matrixBufferDesc, nullptr, &MatrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = InDevice->CreateSamplerState(&samplerDesc, &SampleState);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void FTextureShader::ShutdownShader()
{
	if (SampleState)
	{
		SampleState->Release();
		SampleState = nullptr;
	}

	FShader::ShutdownShader();
}

bool FTextureShader::SetShaderParameters(ID3D11DeviceContext* InDeviceContext, D3DXMATRIX InWorldMatrix, D3DXMATRIX InViewMatrix, D3DXMATRIX InProjectionMatrix)
{
	FShader::SetShaderParameters(InDeviceContext, InWorldMatrix, InViewMatrix, InProjectionMatrix);

	// Set shader texture resource in the pixel shader.
	InDeviceContext->PSSetShaderResources(0, 1, &Texture->Texture);

	return true;
}

void FTextureShader::RenderShader(ID3D11DeviceContext* InDeviceContext, int InIndexCount)
{
	// Set the vertex input layout.
	InDeviceContext->IASetInputLayout(Layout);

	// Set the vertex and pixel shaders for this triangle.
	InDeviceContext->VSSetShader(VertexShader, nullptr, 0);
	InDeviceContext->PSSetShader(PixelShader, nullptr, 0);

	// Set the sampler state in the pixel shader.
	InDeviceContext->PSSetSamplers(0, 1, &SampleState);

	// Render the triangle.
	InDeviceContext->DrawIndexed(InIndexCount, 0, 0);
}

void FTextureShader::SetTexture(FTexture* InTexture)
{
	Texture = InTexture;
}

#endif