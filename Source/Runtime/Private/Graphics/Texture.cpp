/* Copyright 2015 Myles Salholm */
#include "Runtime/Public/Graphics/Texture.h"

FTexture::FTexture()
{
	Texture = nullptr;
}

FTexture::FTexture(const FTexture& Other)
{

}

FTexture::~FTexture()
{

}

bool FTexture::Initialize(ID3D11Device* InDevice, WCHAR* InFile)
{
	HRESULT result;

	// Load the texture
	result = D3DX11CreateShaderResourceViewFromFile(InDevice, InFile, nullptr, nullptr, &Texture, nullptr);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void FTexture::Shutdown()
{
	// Release the texture
	if (Texture)
	{
		Texture->Release();
		Texture = nullptr;
	}
}