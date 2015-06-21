/* Copyright 2015 Myles Salholm */

#include "Runtime/Public/Graphics/Light.h"

FLight::FLight()
{

}

FLight::FLight(const FLight &Other)
{

}

FLight::~FLight()
{

}

void FLight::SetDiffuseColor(float Red, float Green, float Blue, float Alpha)
{
#if DIRECTX
	DiffuseColor = D3DXVECTOR4(Red, Green, Blue, Alpha);
#endif
}

void FLight::SetDirection(float X, float Y, float Z)
{
#if DIRECTX
	Direction = D3DXVECTOR3(X, Y, Z);
#endif
}

#if DIRECTX
D3DXVECTOR4 FLight::GetDiffuseColor()
{
	return DiffuseColor;
}

D3DXVECTOR3 FLight::GetDirection()
{
	return Direction;
}
#endif