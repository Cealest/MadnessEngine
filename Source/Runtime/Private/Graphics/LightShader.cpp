/* Copyright 2015 Myles Salholm */
#include "Runtime/Public/Graphics/LightShader.h"
#include "Runtime/Public/Graphics/Texture.h"

FLightShader::FLightShader() : FTextureShader()
{
#if DIRECTX
	VertexShaderFile = L"C:/MadnessEngine/MadnessEngine/Source/Shaders/Light.vs";
	PixelShaderFile = L"C:/MadnessEngine/MadnessEngine/Source/Shaders/Light.ps";
	VertexFunctionName = "LightVertexShader";
	PixelFunctionName = "LightPixelShader";
	LightBuffer = nullptr;
#endif
}

FLightShader::FLightShader(const FLightShader &Other) : FTextureShader(Other)
{

}

FLightShader::~FLightShader()
{
	
}

#if DIRECTX

bool FLightShader::InitializeShader(ID3D11Device* InDevice, HWND InWindowHandle, WCHAR* InVertexShaderFile, WCHAR* InPixelShaderFile)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;

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

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

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

	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note the ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(SLightBuffer);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer.
	result = InDevice->CreateBuffer(&lightBufferDesc, nullptr, &LightBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void FLightShader::ShutdownShader()
{
	if (LightBuffer)
	{
		LightBuffer->Release();
		LightBuffer = nullptr;
	}

	FTextureShader::ShutdownShader();
}

bool FLightShader::Render(ID3D11DeviceContext* InDeviceContext, int InIndexCount, D3DXMATRIX InWorldMatrix, D3DXMATRIX InViewMatrix, D3DXMATRIX InProjectionMatrix, FLight* InLight)
{
	// Set the shader parameters that it will use for rendering.
	if (!SetShaderParameters(InDeviceContext, InWorldMatrix, InViewMatrix, InProjectionMatrix, InLight))
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(InDeviceContext, InIndexCount);

	return true;
}

bool FLightShader::SetShaderParameters(ID3D11DeviceContext* InDeviceContext, D3DXMATRIX InWorldMatrix, D3DXMATRIX InViewMatrix, D3DXMATRIX InProjectionMatrix, FLight* InLight)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	SMatrixBuffer* matrix;
	SLightBuffer* light;
	unsigned int bufferNumber;

	// Transpose the matrices to prepare them for the shader.
	D3DXMatrixTranspose(&InWorldMatrix, &InWorldMatrix);
	D3DXMatrixTranspose(&InViewMatrix, &InViewMatrix);
	D3DXMatrixTranspose(&InProjectionMatrix, &InProjectionMatrix);

	// Lock the buffer so it can be written to.
	result = InDeviceContext->Map(MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	matrix = (SMatrixBuffer*)mappedResource.pData;
	matrix->World = InWorldMatrix;
	matrix->View = InViewMatrix;
	matrix->Projection = InProjectionMatrix;

	// Unlock the buffer
	InDeviceContext->Unmap(MatrixBuffer, 0);

	bufferNumber = 0;

	InDeviceContext->VSSetConstantBuffers(bufferNumber, 1, &MatrixBuffer);

	// Set shader texture resource in the pixel shader.
	InDeviceContext->PSSetShaderResources(0, 1, &Texture->Texture);

	// Lock the light constant buffer so it can be written to.
	result = InDeviceContext->Map(LightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	light = (SLightBuffer*)mappedResource.pData;

	// Copy the lighting variables to the constant buffer.
	light->DiffuseColor = InLight->GetDiffuseColor();
	light->LightDirection = InLight->GetDirection();
	light->Padding = 0.0f;

	// Unlock the constant buffer.
	InDeviceContext->Unmap(LightBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Set the light constant buffer in the pixel shader with the updated values.
	InDeviceContext->PSSetConstantBuffers(bufferNumber, 1, &LightBuffer);

	return true;
}

void FLightShader::RenderShader(ID3D11DeviceContext* InDeviceContext, int InIndexCount)
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

#endif