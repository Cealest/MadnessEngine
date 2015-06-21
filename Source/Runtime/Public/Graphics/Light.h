/* Copyright 2015 Myles Salholm */
#pragma once

#include "Runtime/Public/Definitions.h"

#if DIRECTX
#include <D3DX10math.h>
#endif

class FLight
{
public:
	FLight();
	FLight(const FLight &Other);
	~FLight();

	void SetDiffuseColor(float Red, float Green, float Blue, float Alpha);
	void SetDirection(float X, float Y, float Z);

#if DIRECTX
	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR3 GetDirection();

private:
	D3DXVECTOR4 DiffuseColor;
	D3DXVECTOR3 Direction;
#endif
};