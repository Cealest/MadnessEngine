/* Copyright 2015 Myles Salholm */

#include "Runtime/Public/Graphics/Model.h"
#include "Runtime/Public/Graphics/Texture.h"
#include "Runtime/Public//Templates/Array.h"

FModel::FModel()
{

#if DIRECTX
	VertexBuffer = nullptr;
	IndexBuffer = nullptr;
	Texture = nullptr;
#endif 
}

FModel::~FModel()
{

}

FModel::FModel(const FModel& Other)
{

}

#if DIRECTX

bool FModel::Initialize(ID3D11Device* InDevice, WCHAR* InTextureFilename)
{
	if (wcscmp(InTextureFilename, L""))
	{
		if (!LoadTexture(InDevice, InTextureFilename))
		{
			return false;
		}
	}

	return InitializeBuffers(InDevice);
}

void FModel::Shutdown()
{
	if (UsesTexture())
	{
		ReleaseTexture();
	}

	ShutdownBuffers();
}

void FModel::Render(ID3D11DeviceContext* InDeviceContext)
{
	RenderBuffers(InDeviceContext);
}

int FModel::GetIndexCount()
{
	return IndexCount;
}

FTexture* FModel::GetTexture() const
{
	return Texture;
}

const bool FModel::UsesTexture() const
{
	return (Texture != nullptr);
}

bool FModel::InitializeBuffers(ID3D11Device* InDevice)
{
	if (UsesTexture())
	{
		return InitializeBuffersTextured(InDevice);
	}
	else
	{
		return InitializeBuffersColored(InDevice);
	}
}

bool FModel::InitializeBuffersColored(ID3D11Device* InDevice)
{
	SColorVertex* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	VertexCount = 3;
	IndexCount = 3;

	vertices = new SColorVertex[VertexCount];
	if (!vertices)
	{
		return false;
	}

	indices = new unsigned long[IndexCount];
	if (!indices)
	{
		return false;
	}

	// Make a triangle, clockwise for front facing
	vertices[0].Position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f); // Bottom Left
	vertices[0].Color = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f); // Purple
	vertices[1].Position = D3DXVECTOR3(0.0f, 1.0f, 0.0f); // Top Middle
	vertices[1].Color = D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f); // Green-Blue
	vertices[2].Position = D3DXVECTOR3(1.0f, -1.0f, 0.0f); // Bottom Right
	vertices[2].Color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f); // Blue

	// Load the index array with data
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	// Setup the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(SColorVertex) * VertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the sub resource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = InDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &VertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * IndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = InDevice->CreateBuffer(&indexBufferDesc, &indexData, &IndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays since the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

bool FModel::InitializeBuffersTextured(ID3D11Device* InDevice)
{
	STextureVertex* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	VertexCount = 3;
	IndexCount = 3;

	vertices = new STextureVertex[VertexCount];
	if (!vertices)
	{
		return false;
	}

	indices = new unsigned long[IndexCount];
	if (!indices)
	{
		return false;
	}

	// Make a triangle, clockwise for front facing
	vertices[0].Position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f); // Bottom Left
	vertices[0].Texture = D3DXVECTOR2(0.0f, 1.0f); // Bottom left
	vertices[1].Position = D3DXVECTOR3(0.0f, 1.0f, 0.0f); // Top Middle
	vertices[1].Texture = D3DXVECTOR2(0.5f, 0.0f); // Top middle
	vertices[2].Position = D3DXVECTOR3(1.0f, -1.0f, 0.0f); // Bottom Right
	vertices[2].Texture = D3DXVECTOR2(1.0f, 1.0f); // Bottom right

	// Load the index array with data
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	// Setup the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(STextureVertex) * VertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the sub resource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = InDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &VertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * IndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = InDevice->CreateBuffer(&indexBufferDesc, &indexData, &IndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays since the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

void FModel::ShutdownBuffers()
{
	// Release the index buffer.
	if (IndexBuffer)
	{
		IndexBuffer->Release();
		IndexBuffer = nullptr;
	}
	// Release the vertex buffer.
	if (VertexBuffer)
	{
		VertexBuffer->Release();
		VertexBuffer = nullptr;
	}
}

void FModel::RenderBuffers(ID3D11DeviceContext* InDeviceContext)
{
	unsigned int stride;
	unsigned int offset;

	if (UsesTexture())
	{
		stride = sizeof(STextureVertex);
	}
	else
	{
		stride = sizeof(SColorVertex);
	}
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	InDeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	InDeviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	
	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	InDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool FModel::LoadTexture(ID3D11Device* InDevice, WCHAR* InFile)
{
	// Create the texture object
	Texture = new FTexture();
	if (!Texture)
	{
		return false;
	}

	// Initialize the texture object
	if (!Texture->Initialize(InDevice, InFile))
	{
		return false;
	}

	return true;
}

void FModel::ReleaseTexture()
{
	if (Texture)
	{
		Texture->Shutdown();
		delete Texture;
		Texture = nullptr;
	}
}

#endif