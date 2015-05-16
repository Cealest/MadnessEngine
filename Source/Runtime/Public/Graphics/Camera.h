/* Copyright 2015 Myles Salholm */
#pragma once

#include "Runtime/Public/Definitions.h"

#if DIRECTX
#include <d3dx10math.h>
#endif

class FCamera
{
public:
	FCamera();
	FCamera(const FCamera& Other);
	~FCamera();

#if DIRECTX
	void SetPosition(float InX, float InY, float InZ);
	void SetRotation(float InX, float InY, float InZ);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render();
	void GetViewMatrix(D3DXMATRIX& OutMatrix);

private:
	float PosX, PosY, PosZ;
	float RotX, RotY, RotZ;
	D3DXMATRIX ViewMatrix;
#endif

};