/* Copyright 2015 Myles Salholm */
#include "Runtime/Public/Graphics/ColorShader.h"

FColorShader::FColorShader()
{
	VertexShader = nullptr;
	PixelShader = nullptr;
	Layout = nullptr;
	MatrixBuffer = nullptr;
}

FColorShader::FColorShader(const FColorShader &Other)
{

}

FColorShader::~FColorShader()
{

}

#if DIRECTX

bool FColorShader::Initialize(ID3D11Device* InDevice, HWND InWindowHandle)
{
	return InitializeShader(InDevice, InWindowHandle, L"C:/MadnessEngine/MadnessEngine/Content/Shaders/Color.vs", L"C:/MadnessEngine/MadnessEngine/Content/Shaders/Color.ps");
}

void FColorShader::Shutdown()
{
	ShutdownShader();
}

bool FColorShader::Render(ID3D11DeviceContext* InDeviceContext, int InIndexCount, D3DXMATRIX InWorldMatrix, D3DXMATRIX InViewMatrix, D3DXMATRIX InProjectionMatrix)
{
	// Set the shader parameters that it will use for rendering.
	if (!SetShaderParameters(InDeviceContext, InWorldMatrix, InViewMatrix, InProjectionMatrix))
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(InDeviceContext, InIndexCount);

	return true;
}

bool FColorShader::InitializeShader(ID3D11Device* InDevice, HWND InWindowHandle, WCHAR* InVertexShaderFile, WCHAR* InPixelShaderFile)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	errorMessage = nullptr;
	vertexShaderBuffer = nullptr;
	pixelShaderBuffer = nullptr;

	// Compile the vertex shader code
	result = D3DX11CompileFromFile(InVertexShaderFile, nullptr, nullptr, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr, &vertexShaderBuffer, &errorMessage, nullptr);
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
	result = D3DX11CompileFromFile(InPixelShaderFile, nullptr, nullptr, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr, &pixelShaderBuffer, &errorMessage, nullptr);
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

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
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

	return true;
}

void FColorShader::ShutdownShader()
{
	if (MatrixBuffer)
	{
		MatrixBuffer->Release();
		MatrixBuffer = nullptr;
	}
	if (Layout)
	{
		Layout->Release();
		Layout = nullptr;
	}
	if (PixelShader)
	{
		PixelShader->Release();
		PixelShader = nullptr;
	}
	if (VertexShader)
	{
		VertexShader->Release();
		VertexShader = nullptr;
	}
}

void FColorShader::OutputShaderErrorMessage(ID3D10Blob* InErrorMessage, HWND InWindowHandle, WCHAR* InShaderFilename)
{
	char* compileErrors;
	unsigned long i;
	SIZE_T bufferSize;
	std::ofstream fout;

	// Get a pointer to the error message buffer
	compileErrors = (char*)(InErrorMessage->GetBufferPointer());

	// Get the length of the message
	bufferSize = InErrorMessage->GetBufferSize();

	// Open a file to output to.
	fout.open(LOGPATH_ABS"shader_error.txt");

	// Write the error message.
	for (i = 0; i < (unsigned long)bufferSize; ++i)
	{
		fout << compileErrors[i];
	}

	// Close the file
	fout.close();

	// Release the error message
	InErrorMessage->Release();
	InErrorMessage = nullptr;

	// Pop up a message on the screen to notify the error.
	MessageBox(InWindowHandle, L"Error compiling shader.  Check shader_error.txt for details.", InShaderFilename, MB_OK);
}

bool FColorShader::SetShaderParameters(ID3D11DeviceContext* InDeviceContext, D3DXMATRIX InWorldMatrix, D3DXMATRIX InViewMatrix, D3DXMATRIX InProjectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	SMatrixBuffer* matrix;
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

	return true;
}

void FColorShader::RenderShader(ID3D11DeviceContext* InDeviceContext, int InIndexCount)
{
	// Set the vertex input layout
	InDeviceContext->IASetInputLayout(Layout);

	// Set the vertex and pixel shaders for this triangle
	InDeviceContext->VSSetShader(VertexShader, nullptr, 0);
	InDeviceContext->PSSetShader(PixelShader, nullptr, 0);

	// Render the triangle
	InDeviceContext->DrawIndexed(InIndexCount, 0, 0);
}

#endif