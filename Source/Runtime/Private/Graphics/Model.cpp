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
	Mesh = nullptr;
#endif 
}

FModel::~FModel()
{

}

FModel::FModel(const FModel& Other)
{

}

#if DIRECTX

bool FModel::Initialize(ID3D11Device* InDevice, const char* InModelFilename, WCHAR* InTextureFilename)
{
	if (strcmp(InModelFilename, ""))
	{
		if (!LoadModel(InModelFilename))
		{
			return false;
		}
	}
	else // No model name, can't init
		return false;

	if (wcscmp(InTextureFilename, L""))
	{
		if (!LoadTexture(InDevice, InTextureFilename))
		{
			return false;
		}
	}
	else
		return InitializeBuffers(InDevice, EShader::Color);

	return InitializeBuffers(InDevice, EShader::DiffuseLight);
}

void FModel::Shutdown()
{
	if (UsesTexture())
	{
		ReleaseTexture();
	}

	ShutdownBuffers();

	ReleaseModel();
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

bool FModel::InitializeBuffers(ID3D11Device* InDevice, EShader::Type InShaderType)
{
	ActiveShaderType = InShaderType;
	switch (ActiveShaderType)
	{
	case EShader::DiffuseLight:
		return InitializeBuffersLitTextured(InDevice);
	case EShader::Texture:
		return InitializeBuffersTextured(InDevice);
	case EShader::Color:
	default:
		return InitializeBuffersColored(InDevice);
	}
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
	D3DColorVertex* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	vertices = new D3DColorVertex[VertexCount];
	if (!vertices)
	{
		return false;
	}

	indices = new unsigned long[IndexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data
	for (int i = 0; i < VertexCount; ++i)
	{
		vertices[i].Position = D3DXVECTOR3(Mesh->Vertices[i].LocX, Mesh->Vertices[i].LocY, Mesh->Vertices[i].LocZ);
		vertices[i].Color = D3DXVECTOR4(Mesh->Vertices[i].NormalX, Mesh->Vertices[i].NormalY, Mesh->Vertices[i].NormalZ, 1.0f);

		indices[i] = i;
	}

	// Setup the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(D3DColorVertex) * VertexCount;
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
	D3DTextureVertex* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	vertices = new D3DTextureVertex[VertexCount];
	if (!vertices)
	{
		return false;
	}

	indices = new unsigned long[IndexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data
	for (int i = 0; i < VertexCount; ++i)
	{
		vertices[i].Position = D3DXVECTOR3(Mesh->Vertices[i].LocX, Mesh->Vertices[i].LocY, Mesh->Vertices[i].LocZ);
		vertices[i].Texture = D3DXVECTOR2(Mesh->Vertices[i].TexU, Mesh->Vertices[i].TexV);

		indices[i] = i;
	}

	// Setup the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(D3DTextureVertex) * VertexCount;
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

bool FModel::InitializeBuffersLitTextured(ID3D11Device* InDevice)
{
	D3DLitTextureVertex* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	vertices = new D3DLitTextureVertex[VertexCount];
	if (!vertices)
	{
		return false;
	}

	indices = new unsigned long[IndexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data
	for (int i = 0; i < VertexCount; ++i)
	{
		vertices[i].Position = D3DXVECTOR3(Mesh->Vertices[i].LocX, Mesh->Vertices[i].LocY, Mesh->Vertices[i].LocZ);
		vertices[i].Texture = D3DXVECTOR2(Mesh->Vertices[i].TexU, Mesh->Vertices[i].TexV);
		vertices[i].Normal = D3DXVECTOR3(Mesh->Vertices[i].NormalX, Mesh->Vertices[i].NormalY, Mesh->Vertices[i].NormalZ);

		indices[i] = i;
	}

	// Setup the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(D3DLitTextureVertex) * VertexCount;
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

	switch (ActiveShaderType)
	{
	case EShader::DiffuseLight:
		stride = sizeof(D3DLitTextureVertex);
		break;
	case EShader::Texture:
		stride = sizeof(D3DTextureVertex);
		break;
	case EShader::Color:
		stride = sizeof(D3DColorVertex);
		break;
	default:
		stride = sizeof(D3DVertex);
		break;
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

bool FModel::LoadModel(const char* InFile)
{
	std::ifstream fileIn;
	char input;
	int i;

	// Open the file.
	fileIn.open(InFile);
	if (fileIn.fail())
	{
		return false;
	}

	// Read the value of the vertex count.
	fileIn.get(input);
	while (input != ':')
	{
		fileIn.get(input);
		if (input == '\0') // Reached end of file
			return false;
	}
	fileIn >> VertexCount;

	// Set the number of indices to be the same as the vertex count
	IndexCount = VertexCount;

	// Create the model using the vertex count that was read in.
	Mesh = new SMesh(VertexCount);
	if (!Mesh)
	{
		return false;
	}

	// Read the beginning of the data.
	fileIn.get(input);
	while (input != ':')
	{
		fileIn.get(input);
		if (input == '\0') // Reached end of file
			return false;
	}
	fileIn.get(input);
	fileIn.get(input);

	// Read the vertex data.
	for (i = 0; i < VertexCount; ++i)
	{
		fileIn >> Mesh->Vertices[i].LocX >> Mesh->Vertices[i].LocY >> Mesh->Vertices[i].LocZ;
		fileIn >> Mesh->Vertices[i].TexU >> Mesh->Vertices[i].TexV;
		fileIn >> Mesh->Vertices[i].NormalX >> Mesh->Vertices[i].NormalY >> Mesh->Vertices[i].NormalZ;
	}

	// Close the file.
	fileIn.close();

	return true;
}

void FModel::ReleaseModel()
{
	if (Mesh)
	{
		delete Mesh;
		Mesh = nullptr;
	}
}

#endif